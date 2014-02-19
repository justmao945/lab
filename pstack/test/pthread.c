#include <pthread.h>
#include "pstack.h"

void hello() {
    pstack();
}

void world() {
    hello();
}

void* do_it (void* data) {
    world();
    return 0;
}


int main () {
    int i = 0;
    pthread_t idgpr[5] = {0};
    void* res = 0;

    rstack();
    
    for(i = 0; i < 5; ++i) {
        pthread_create(&idgpr[i], 0, do_it, 0);
    }
    for(i = 0; i < 5; ++i) {
        pthread_join(idgpr[i], &res);
    }
    return 0;
}
