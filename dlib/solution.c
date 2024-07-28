#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int (*some_function)(int);

int init_library(const char* lib_name, const char* fn_name) {
    
    char lib[128];
    if (lib_name[0] != '.')
        snprintf(lib, 127, "./%s", lib_name);
    else    
        snprintf(lib, 127, "%s", lib_name);

    void *handle = dlopen(lib, RTLD_LAZY);
    if(handle == NULL) 
        return 0;
        
    some_function = (int (*)(int)) dlsym(handle, fn_name);
    if (some_function == NULL)
        return 0;

    return 1;
}

int main(int argc, char** argv) {


    if (argc != 4 )  {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    

    int number = atoi(argv[3]);
    if(init_library(argv[1], argv[2])) {
       printf("%d\n", some_function(number));
    }
    else
        printf("Error initialisation dynamic library.\n");

    return 0;
}

