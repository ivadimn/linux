#include <stdio.h>
#include <dlfcn.h>

void (*hello_message)(const char*);

int init_library() {
    void *handle = dlopen("./libhello.so", RTLD_LAZY);
    if(handle == NULL) 
        return 0;
        
    hello_message = (void (*)(const char*)) dlsym(handle, "hello_message");
    if (hello_message == NULL)
        return 0;

    return 1;
}

int main() {


    if(init_library())
        hello_message("Vasia");
    else
        printf("Error initialisation dynamic library.");

    return 0;
}

