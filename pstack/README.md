StackTrace
==========

stacktrace for C/C++


Example
==========
* Print stack trace manually

        #include "pstack.h"
        
        void where_to_printTrace() {
            /* wwww */
            pstack();
        }
        
        int main() {
            rstack(); /* for default fatal error handler */
            /* ....  */
            return 0;
        }

* Print stack trace when registered signals raised

        #include "pstack.h"
        
        void segfault() {
            int* ptr = 0;
            int y = *ptr;
        }
        
        void papa() {
            segfault();
        }
        
        int main() {
            rstack();
            papa();
            return 0;
        }


Requirement & Usage
=========
0. Only tested on Linux(Ubuntu/Debian).
1. `glibc >= 2.1` for `backtrace()` and `dladdr()`.
2. Compile program with `-g` to generate debug info for symboilzer.
3. Link program with `-rdynamic` to resolve symbols and `-ldl` for `dladdr()`.
4. cxxabi to demangle in C++ (Optional).


Attention
=========
If you want to show name of all symbols, don not use `static` functions
and don not add `-fvisibility=hidden` to your compiler.

Symbolizer
=========


[1]: http://llvm.org/docs/doxygen/html/Unix_2Signals_8inc_source.html
[2]: http://tldp.org/LDP/abs/html/exitcodes.html
[3]: http://panthema.net/2008/0901-stacktrace-demangled/
[4]: http://www.comptechdoc.org/os/linux/programming/linux_pgsignals.html
[5]: http://en.cppreference.com/w/c/program/SIG_types
[6]: http://stackoverflow.com/questions/231912/what-is-the-difference-between-sigaction-and-signal
[8]: http://lists.nongnu.org/archive/html/libunwind-devel/2011-08/msg00054.html
[9]: http://stackoverflow.com/questions/1765969/unable-to-locate-definition-of-etext-edata-end
[10]: http://stackoverflow.com/questions/3565232/how-to-programmatically-get-the-address-of-the-heap-on-linux
