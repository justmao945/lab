#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// return uniform rand number in [0,n) n > 0
int randn(int n) {
    assert(n <= RAND_MAX && n > 0);
    int v;
    // max is a big number that (m+1) can be divied by n
    // e.g. we want to gen rand in [0, 3)
    // now we have number 0,1,2,3,4,5,6,7,8,9...
    //  we can choose max as 9 - 1, and ignore all > 8, then we have
    //  0,1,2,3,4,5,6,7,8
    //%                 3
    //= 0 1 2 0 1 2 0 1 2
    int max = RAND_MAX / n * n - 1;
    // ignore all numbers out of max
    while( (v = rand()) > max ) {}
    // now, we can get the uniform rand number
    return v % n;
}

int main() {
    const int N = 11, M = 1000000;
    int i, j;
    int cnt[N] = {0};
    for(i = 0; i < M; i++) {
        cnt[randn(N)]++;
    }
    puts("uniform: ");
    for(i = 0; i < N; i++) {
        printf("%d ", cnt[i]);
    }
    puts("");

    // may have little impact when only use rand() % N ?

    return 0;
}
