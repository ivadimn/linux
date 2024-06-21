#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int (*some_function)(int);

int init_library(const char* lib_name, const char* fn_name) {
    
     void *handle = dlopen(lib_name, RTLD_LAZY);
    if(handle == NULL) 
        return 0;
        
    some_function = (int (*)(int)) dlsym(handle, fn_name);
    if (some_function == NULL)
        return 0;

    return 1;
}

int main(int argc, char** argv) {

    int number = atoi(argv[3]);
    if(init_library(argv[1], argv[2])) {
       printf("%d\n", some_function(number));
    }
    else
        printf("Error initialisation dynamic library.\n");

    return 0;
}

