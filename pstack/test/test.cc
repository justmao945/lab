

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "pstack.h"

struct T {

int func1(int sig) {
    int *p = 0;
    raise(sig);
    int y = *p;
    return 1;
}

int func2(int sig) {
    int x = 1;
    func1(sig);
    int  y = 23;
    return 1;
}

int func3(int sig) {
    return func2(sig);
}

};

int main(int argc, char** argv) {
    T t;
    stdx::rstack();
    if(argc != 2) {
        printf("usage: %s <signo>\n", argv[0]);
    }
    t.func3(atoi(argv[1]));
    return 0;
}
