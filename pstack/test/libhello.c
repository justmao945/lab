
#include <stdio.h>
#include "pstack.h"

void* mallocx() {
    pstack();
    return 0;
}

static void hbt_read() {
    mallocx();
}

static void hbt_write() {
    mallocx();
}

struct IO_methods {
    void(*read)();
    void(*write)();
} io_methods = { hbt_read, hbt_write };

struct IO {
    struct IO_methods *methods;
};

void hello() {
    pstack();
}

int f1() {
    hello();
    return 1;
}

int(*pf)() = f1;

int f2() {
    struct IO iof;
    iof.methods = &io_methods;
    iof.methods->read();
    iof.methods->write();
    return pf();
}

