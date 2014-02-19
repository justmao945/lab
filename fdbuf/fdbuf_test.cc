
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "fdbuf.h"

using namespace std;
using namespace stdx;

bool cmp(const char* a, const char* b) {
    ifstream fa(a), fb(b);
    while(fa && fb) {
        if(fa.get() != fb.get()) {
            return false;
        }
    }
    return fa.eof() && fb.eof();
}

void test(const char* in, const char* out) {
    cout << "Case [" << in << "] ...." << flush;
    fdbuf fdin (::open(in,  O_RDONLY));
    fdbuf fdout(::open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    istream fsin(&fdin);
    ostream fsout(&fdout);
    fsout << fsin.rdbuf() << flush;
    ::close(fdin.fd());
    ::close(fdout.fd());
    if( cmp(in, out) ) {
        cout << "[pass]" << endl;
    }else {
        cout << "[fail]" << endl;
    }
}

int main() {
    const char* list[] = {
        "data/small.in",  "data/small.out",
        "data/medium.in", "data/medium.out",
    };
    
    for(size_t i = 0; i < sizeof(list)/sizeof(void*); i += 2) {
        ::remove(list[i+1]);
        test(list[i], list[i+1]);
    }
    return 0;
}
