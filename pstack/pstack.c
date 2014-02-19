
/*
 * Add dlfcn.h to get dynamic library symbols.
 */
#define STACK_TRACE_HAVE_DLFCN_H 1

/*
 * Add cxxabi.h to demangle C++ symbol names.
 */
#ifdef __cplusplus
# define STACK_TRACE_HAVE_CXXABI_H 0
#endif


#if STACK_TRACE_HAVE_DLFCN_H
# ifndef _GNU_SOURCE
#  define _GNU_SOURCE /* Glibc extensions for Dl_info */
# endif
#endif

#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <execinfo.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "pstack.h"

#if STACK_TRACE_HAVE_DLFCN_H
# include <dlfcn.h>
#endif

#define STACK_TRACE_HAVE_CXA_DEMANGLE 1
#if STACK_TRACE_HAVE_CXXABI_H
# include <cxxabi.h>
#elif defined(__cplusplus)
namespace __cxxabiv1 {
  extern "C" {
    char* __cxa_demangle(const char*, char*, size_t*, int*);
  }
}
namespace abi = __cxxabiv1;
#else
  #undef  STACK_TRACE_HAVE_CXA_DEMANGLE /* no __cxa_demangle */
#endif

/* backtrace depth */
#define STACK_TRACE_SIZE 256

/* monitor singals */
#define STACK_TRACE_SIGNALS 9

/*
 * A list of signals, only contains crashes.
 * NOTE: Edit this list and STACK_TRACE_SIGNALS to monitor different signals
 */
static const int siglist[STACK_TRACE_SIGNALS] = {
    SIGINT, /* 2 */
    SIGQUIT,/* 3 */
    SIGILL, /* 4 */
    SIGABRT,/* 6 */
    SIGBUS, /* 7 */
    SIGFPE, /* 8 */
    SIGSEGV,/* 11 */
    SIGPIPE,/* 13 */
    SIGTERM,/* 15 */
};

/*
 * To store registered actions.
 * Adding `volatile` tells compiler this may change asynchronously.
 */
static struct sigaction oldacts[32]; /* 32 > SIGTERM + 1 */

/*
 * Test registered or not
 */
static int registered;

/*
 * `man 3 end`.
 * Please note that "end" may be overwritten by dlopen, but "_end" not.
 */
extern char _end;
static long addroff(Dl_info* dlinfo) {
    long off = 0;
    assert(dlinfo);
    if((char*)dlinfo->dli_saddr > &_end) { /* shared object */
        off = (char*)dlinfo->dli_saddr - (char*)dlinfo->dli_fbase;
    }else { /* main executable */
        off = (char*)dlinfo->dli_saddr - (char*)0;
    }
    return off;
}

/*
 * Print address
 */
static void fpraddr(FILE* stream, void *addr) {
    fprintf(stream, "%#0*lx", (int)sizeof(void*) * 2 + 2, (unsigned long)addr);
}

/*
 * Lock fd to sync in multiple processes.
 * Nested lock is OK.
 */
static unsigned stderr_lockers;

static void lock_stderr() {
    struct flock lock;
    if(stderr_lockers == 0) {
        lock.l_type   = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start  = 0;
        lock.l_len    = 0;
        /* Uses F_SETLKW to wait the race */
        while(fcntl(STDERR_FILENO, F_SETLKW, &lock) == -1) {
            if(errno == EINTR) {
                continue;
            }else {
                perror("fcntl");
                return;
            }
        }
    }
    ++stderr_lockers;
}

/*
 * Relase lock.
 */
static void unlock_stderr() {
    struct flock lock;
    if(stderr_lockers > 0) {
        --stderr_lockers;
    }
    if(stderr_lockers == 0) {
        lock.l_type   = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start  = 0;
        lock.l_len    = 0;
        while(fcntl(STDERR_FILENO, F_SETLK, &lock) == -1) {
            if(errno == EINTR) {
                continue; 
            } else {
                perror("fcntl");
                return;
            }
        }
    }
}

#ifdef __cplusplus
namespace stdx {
#endif /* end of __cplusplus */

/* FIXME: use signal handler safe functions, see man 7 signal */
void pstack() {
    void* stk[STACK_TRACE_SIZE];
    char strhex[33] = {0};
    int depth = backtrace(stk, STACK_TRACE_SIZE);

#if STACK_TRACE_HAVE_DLFCN_H
    int i = 0, width = 0, nwidth = 0, w1 = 0, w2 = 0, res = 0;
    char *sname = 0;
    Dl_info dlinfo;
#endif /* end of STACK_TRACE_HAVE_DLFCN_H */

    /* Lock stderr in multiple processes */
    lock_stderr();
    /* flush anything whether buffered or not */
    fflush(stderr);

#if STACK_TRACE_HAVE_DLFCN_H
    for(i = 0; i < depth; ++i) {
        dladdr(stk[i], &dlinfo);
        w1 = strlen(dlinfo.dli_fname);
        snprintf(strhex, 33, "%lx", addroff(&dlinfo));
        w2 = strlen(strhex);
        nwidth = w1 + w2 + 2; /* a.out+0x1234 */
        if( nwidth > width) {
            width = nwidth;
        }
    }
    fprintf(stderr, "Backtrack thread %ld (most recent call first):\n", syscall(SYS_gettid));
    for(i = 0; i < depth; ++i) {
        dladdr(stk[i], &dlinfo);
        fprintf(stderr, "  %-2d ", i);
        fpraddr(stderr, stk[i]);
        w1 = width - strlen(dlinfo.dli_fname);
        fprintf(stderr, " %s + %#-*lx ", dlinfo.dli_fname, w1, addroff(&dlinfo));
        if(dlinfo.dli_sname) {
            fputc(' ', stderr);
#if STACK_TRACE_HAVE_CXA_DEMANGLE
            sname = abi::__cxa_demangle(dlinfo.dli_sname, 0, 0, &res);
#else
            res = 0;
            sname = 0;
#endif /* end of STACK_TRACE_HAVE_CXXABI_H */
            if(sname) {
                fputs(sname, stderr);
            }else {
                fputs(dlinfo.dli_sname, stderr);
            }
            free(sname);
        }
        fputc('\n', stderr);
    }
#else
    backtrace_symbols_fd(stk, depth, STDERR_FILENO);
#endif /* end of STACK_TRACE_HAVE_DLFCN_H */
    fflush(stderr);
    unlock_stderr();
}

void pstack1(int signo) {
    struct sigaction oact;
    lock_stderr();
    pstack();
    fprintf(stderr, "  Signal %d catched\n", signo);
    fflush(stderr);
    unlock_stderr();
    sigaction(signo, &oldacts[signo], &oact); /* restore saved default handler */
    raise(signo);
}

void rstack() {
    size_t i = 0;
    struct sigaction act;
    if(registered) {
        return;
    }
    registered = 1;
    memset(&act, 0, sizeof(act));
    act.sa_handler = pstack1;
    for(i = 0; i < STACK_TRACE_SIGNALS; ++i) {
        sigaction(siglist[i], &act, &oldacts[siglist[i]]);
    }
}

void urstack() {
    size_t i = 0;
    struct sigaction oact;
    if(! registered) {
        return;
    }
    for(i = 0; i < STACK_TRACE_SIGNALS; ++i) {
        sigaction(siglist[i], &oldacts[siglist[i]], &oact);
    }
    registered = 0;
}

#ifdef __cplusplus
}
#endif
