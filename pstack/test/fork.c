
#include <stdio.h>
#include <unistd.h>
#include <pstack.h>

void trace() {
    int i = 0;
    for(; i < 5; ++i) {
        pid_t pid = fork();
        if(pid == 0) { // child
            int *p = 0;
            *p = 1;
        }
    }
}

void world() {
    trace();
}

void hello() {
    world();
}

int main () {
    rstack();
    hello();
    return 0;
}
