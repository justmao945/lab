
void* mallocx() {
    int *p = 0;
    *p = 2;
    return 0;
}


int f2() {
    mallocx();
    return 1;
}

