
#ifndef STDX_FDBUF_H
#define STDX_FDBUF_H

#include <unistd.h>
#include <fcntl.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <streambuf>
#include <locale>

namespace stdx {

//
// > This buffer doesn't use exception to handle errors.
// > This buffer will *NOT* close file descriptor when destructed.
template<class CharT, class Traits = std::char_traits<CharT> >
class basic_fdbuf: public std::basic_streambuf<CharT, Traits> {
public:
    typedef CharT    char_type;
    typedef Traits   traits_type;
    typedef typename traits_type::int_type int_type;
    typedef typename traits_type::pos_type pos_type;
    typedef typename traits_type::off_type off_type;

    basic_fdbuf();
    explicit basic_fdbuf(int fd);   // Construct from Unix file descriptor
    virtual ~basic_fdbuf();         // *NOT* close fd
    void swap(basic_fdbuf& rhs);

    bool attach(int fd);

    // get fd
    int fd() const { return _fd; }

protected:
    virtual int_type underflow();   // for input to refill buffer
    virtual int_type pbackfail(int_type c = traits_type::eof()); // for input to unget
    virtual int_type overflow (int_type c = traits_type::eof()); // for output to flush buffer
    virtual std::basic_streambuf<char_type, traits_type>* setbuf(char_type* s, std::streamsize n); // use external buffer
    virtual pos_type seekoff(off_type off, std::ios_base::seekdir way,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    virtual pos_type seekpos(pos_type sp,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    virtual int sync(); // for output to flush
    virtual void imbue(const std::locale& loc);

private:
    static const size_t _BUFSZ = 8192;  // default internal buffer size
    int         _fd;        // file descriptor
    int         _fdfl;      // GETFL from file descriptor
    char_type*  _buf;       // buffer pointer
    size_t      _bufsz;     // buffer size
    bool        _bufown;    // buffer owner flag

    static const int _CM_RD = 0x01;
    static const int _CM_WR = 0x10;
    int         _cm;        // internal read/write flag
};

template<class CharT, class Traits>
inline void
swap(basic_fdbuf<CharT, Traits>& x, basic_fdbuf<CharT, Traits>& y) {
    x.swap(y);
}

// Constructor
template<class CharT, class Traits>
basic_fdbuf<CharT, Traits>::basic_fdbuf() :
        _fd(-1),
        _fdfl(0),
        _buf(0),
        _bufsz(0),
        _bufown(false),
        _cm(0) { }

template<class CharT, class Traits>
basic_fdbuf<CharT, Traits>::basic_fdbuf(int fd) :
        _fd(fd),
        _fdfl(0),
        _buf(0),
        _bufsz(0),
        _bufown(false),
        _cm(0) {
    attach(fd);
}

// Destructor
template<class CharT, class Traits>
basic_fdbuf<CharT, Traits>::~basic_fdbuf() {
    sync(); // do synchronize before leave
    if(_bufown) {
        delete[] _buf;
    }
}

template<class CharT, class Traits>
bool
basic_fdbuf<CharT, Traits>::attach(int fd) {
    assert(fd >= 0 && "invalid file descriptor");
    _fd = fd;
    while( (_fdfl = ::fcntl(_fd, F_GETFL)) == -1 && errno == EINTR ) { }
    setbuf(0, _BUFSZ); // init internal buffer with _BUFSZ
    return _fdfl != -1;
}

template<class CharT, class Traits>
void
basic_fdbuf<CharT, Traits>::swap(basic_fdbuf<CharT, Traits>& rhs) {
    std::basic_streambuf<CharT, Traits>::swap(rhs);
    std::swap(_fd,      rhs._fd);
    std::swap(_fdfl,    rhs._fdfl);
    std::swap(_buf,     rhs._buf);
    std::swap(_bufsz,   rhs._bufsz);
    std::swap(_bufown,  rhs._bufown);
    std::swap(_cm,      rhs._cm);
}

// Remakrs: The public members of basic_streambuf call this virtual function only
//          if gptr() is null or gptr() >= egptr()
// Returns: traits::to_int_type(c), where c is the first character of the pending
//          sequence, without moving the input sequence position past it. If the
//          pending sequence is null then the function returns
template<class CharT, class Traits>
typename basic_fdbuf<CharT, Traits>::int_type
basic_fdbuf<CharT, Traits>::underflow() {
    if(_fd < 0) {
        return traits_type::eof();
    }
    if(! (_cm & _CM_RD)) { // reset buffer pointers when change from other mode into reading
        this->setp(0, 0);
        this->setg(_buf, _buf + _bufsz, _buf + _bufsz);
        _cm = _CM_RD; // keep mode flag
    }
    char_type chbuf;
    if(this->gptr() == 0) { // empty buffer
        this->setg(&chbuf, &chbuf + 1, &chbuf + 1);
    }
    // reserved for ungeting chars within 4 bytes
    const size_t ungetsz = std::min<size_t>((this->egptr() - this->eback()) / 2, 4);
    int_type c = traits_type::eof();
    if(this->gptr() == this->egptr()) { // need to read new chars
        ::memmove(this->eback(), this->egptr() - ungetsz, ungetsz * sizeof(char_type));
        size_t rn = 0, an = static_cast<size_t>(this->egptr() - this->eback() - ungetsz) * sizeof(char_type);
        while(rn < an) {
            ssize_t n = ::read(_fd, this->eback() + ungetsz + rn, an - rn);
            if(n == -1) {
                if(errno == EINTR) {
                    continue;
                }else {
                    break; // can not handled error
                }
            }else if(n == 0) { // EOF
                break; // break and return c(aka EOF)
            }else {
                rn += n;
            }
        }
        if(rn != 0) { // only reset when read someting, otherwise(e.g EOF) do nothing
            this->setg(this->eback(), this->eback() + ungetsz, this->eback() + ungetsz + rn / sizeof(char_type));
            c = traits_type::to_int_type(*this->gptr());
        }
    } else { // read c from buffer
        c = traits_type::to_int_type(*this->gptr());
    }
    if(this->eback() == &chbuf) {
        this->setg(0, 0, 0); // reset to empty buffer
    }
    return c;
}

// Remarks: The public functions of basic_streambuf call this virtual function only when gptr() is null,
//          gptr() == eback(), or traits::eq(traits::to_char_type(c),gptr()[-1]) returns false.
// Returns: traits::eof() to indicate failure. Failure may occur because the input sequence could not
//          be backed up, or if for some other reason the pointers could not be set consistent with the constraints.
//          pbackfail() is called only when put back has really failed.
template<class CharT, class Traits>
typename basic_fdbuf<CharT, Traits>::int_type
basic_fdbuf<CharT, Traits>::pbackfail(int_type c) {
    if(_fd >= 0 && this->eback() < this->gptr()) { // need guard
        if(traits_type::eq_int_type(c, traits_type::eof())) {
            this->gbump(-1);
            return traits_type::not_eof(c);
        }
        int mode = O_ACCMODE & _fdfl;
        if(mode == O_WRONLY || mode == O_RDWR ||
                traits_type::eq(traits_type::to_char_type(c), this->gptr()[-1])) {
            this->gbump(-1);
            *this->gptr() = traits_type::to_char_type(c);
            return c;
        }
    }
    return traits_type::eof();
}

// Effects: Consumes some initial subsequence of the characters of the pending sequence. The pending
//          sequence is defined as the concatenation of
//              a) if pbase() is NULL then the empty sequence otherwise, pptr() - pbase() characters beginning at pbase().
//              b) if traits::eq_int_type(c,traits::eof()) returns true, then the empty sequence otherwise,
// Returns: traits::eof() or throws an exception if the function fails.
template<class CharT, class Traits>
typename basic_fdbuf<CharT, Traits>::int_type
basic_fdbuf<CharT, Traits>::overflow(int_type c) {
    if(_fd < 0) {
        return traits_type::eof();
    }
    if(! (_cm & _CM_WR)) { // reset buffer pointers when change from other mode into writing
        this->setg(0, 0, 0);
        if(_buf && _bufsz >= 1) {
            this->setp(_buf, _buf + _bufsz - 1); // reserve one box for @c
        }else {
            this->setp(0, 0);
        }
        _cm = _CM_WR; // keep mode flag
    }
    // saved buffer pointers to be restored if flushed into device
    char_type chbuf, *pb_save = this->pbase(), *ep_save = this->epptr();
    if(! traits_type::eq_int_type(c, traits_type::eof())) {
        if(this->pptr() == 0) {
            this->setp(&chbuf, &chbuf + 1); // empty buffer
        }
        *this->pptr() = traits_type::to_char_type(c); // write to buffer
        this->pbump(1); // let pptr() go ahead
    }
    if(this->pptr() != this->pbase()) { // need to flush to device
        size_t wn = 0, an = sizeof(char_type) * static_cast<size_t>(this->pptr() - this->pbase());
        while(wn < an) {
            ssize_t n = ::write(_fd, this->pbase() + wn, an - wn);
            if(n == -1) {
                if(errno == EINTR) {
                    continue;
                }else {
                    return traits_type::eof(); // can not handled error
                }
            }else {
                wn += n;
            }
        }
        this->setp(pb_save, ep_save); // flushed successfully, reset buffer pointers
    }
    return traits_type::not_eof(c);
}

// Effects: implementation-defined, except that setbuf(0,0) has no effect.
template<class CharT, class Traits>
std::basic_streambuf<CharT, Traits>*
basic_fdbuf<CharT, Traits>::setbuf(char_type *s, std::streamsize n) {
    this->setg(0, 0, 0);
    this->setp(0, 0);
    if(_bufown) {
        delete[] _buf;
    }
    if(! s && n) {
        _buf    = new char_type[n];
        _bufown = true;
    } else {
        _buf    = s;
        _bufown = false;
    }
    _bufsz  = n;
    _cm = 0; // reset internal flag to reset buffer pointers
    return this;
}

template<class CharT, class Traits>
typename basic_fdbuf<CharT, Traits>::pos_type
basic_fdbuf<CharT, Traits>::seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode which) {
    if(_fd < 0 || sync()) { // here we need to synchronize
        return pos_type(off_type(-1));
    }
    int whence;
    switch(way) {
    case std::ios_base::beg:
        whence = SEEK_SET;
        break;
    case std::ios_base::cur:
        whence = SEEK_CUR;
        break;
    case std::ios_base::end:
        whence = SEEK_END;
        break;
    default:
        return pos_type(off_type(-1));
    }
    if(::lseek(_fd, off, whence) == -1) {
        return pos_type(off_type(-1));
    }
    return pos_type(off_type(::lseek(_fd, 0, SEEK_CUR)));
}

template<class CharT, class Traits>
typename basic_fdbuf<CharT, Traits>::pos_type
basic_fdbuf<CharT, Traits>::seekpos(pos_type sp, std::ios_base::openmode which) {
    if(_fd < 0 || sync()) { // need to synchronize
        return pos_type(off_type(-1));
    }
    return pos_type(off_type(::lseek(_fd, sp, SEEK_SET)));
}

// Effects: Synchronizes the controlled sequences with the arrays. That is, if pbase() is non-null the
//          characters between pbase() and pptr() are written to the controlled sequence. The pointers may
//          then be reset as appropriate.
// Returns: -1 on failure. What constitutes failure is determined by each derived class
template<class CharT, class Traits>
int
basic_fdbuf<CharT, Traits>::sync() {
    if(_fd < 0) {
        return 0;
    }
    if(_cm & _CM_WR) {
        // just flush to device
        if(overflow() == traits_type::eof() ) {
            return -1;
        }
    }else if(_cm & _CM_RD) {
        if( ::lseek(_fd, this->gptr() - this->egptr(), SEEK_CUR) == -1 ) {
            return -1;
        }
        this->setg(0, 0, 0);
        _cm = 0; // need to reset buffer pointer
    }
    return 0;
}

// ...
template<class CharT, class Traits>
void
basic_fdbuf<CharT, Traits>::imbue(const std::locale& loc) {
    assert(0 && "unsupported"); // unsupported
}

// define class
typedef basic_fdbuf<char> fdbuf;

// typedef basic_fdbuf<wchar_t> wfdbuf unsupported

} // end of stdx

#endif  // end of STDX_FDBUF_H
// vim: set ft=cpp :
