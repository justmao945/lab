
#include <stdio.h>
#include <dlfcn.h>

typedef int (*HelloFunc)();

int main() {
    void* dll = dlopen("libhello.so", RTLD_LAZY);
    char*  error = 0;
    if(! dll) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
    dlerror();
    HelloFunc f = (HelloFunc) dlsym(dll, "f2");
    if( (error = dlerror()) != 0 ) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }
    printf("%d\n", f());
    dlclose(dll);
    return 0;
}
