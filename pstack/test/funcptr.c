#include "pstack.h"

int f1() {
    int *p = 0;
    *p = 0;
    return 1;
}


int f2() {
    int(*f)() = f1;

    return f();
}

int main() {
    rstack();
    int(*f)() = f2;
    f();
    return 0;
}
