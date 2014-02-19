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
