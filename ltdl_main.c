#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ltdl.h>

int main(int argc, char *argv[]) {
    lt_dlhandle handle;
    lt_dlhandle handle2;
    void (*formula_printf)(const char*);

    lt_dlinit();

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Improper number of arguments : %s .\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "HOCl") == 0) {
        handle = lt_dlopen("./chlorine.so");
        if (argc == 3 && strcmp(argv[2], "HOBr") == 0) {
            handle2 = lt_dlopen("./bromine.so");
        }
    } else if (strcmp(argv[1], "HOBr") == 0) {
        handle = lt_dlopen("./bromine.so");
        if (argc == 3 && strcmp(argv[2], "HOCl") == 0) {
            handle2 = lt_dlopen("./chlorine.so");
        }
    } else {
        fprintf(stderr, "Error: unknown formula: %s\n", argv[1]);
        return EXIT_FAILURE;
    } 

    if (!handle) {
        // fail to load the library
        fprintf(stderr, "Error: %s\n", lt_dlerror());
        return EXIT_FAILURE;
    }
    printf("Dlsym1?\n");
    *(void**)(&formula_printf) = lt_dlsym(handle, "chemical_name");
    if (!formula_printf) {
        // no such symbol 
        fprintf(stderr, "Error: %s\n", lt_dlerror());
        lt_dlclose(handle);
        return EXIT_FAILURE;
    }

    formula_printf(argv[1]);
    lt_dlclose(handle);

    if (argc == 3) {

        if (!handle2) {
           // fail to load the library 
            fprintf(stderr, "Error: %s\n", lt_dlerror());
            return EXIT_FAILURE;
        }/

        *(void**)(&formula_printf) = lt_dlsym(handle2, "chemical_name");

        if (!formula_printf) {
            // no such symbol 
            fprintf(stderr, "Error: %s\n", lt_dlerror());
            lt_dlclose(handle2);
            return EXIT_FAILURE;
        }

        formula_printf(argv[2]);
        lt_dlclose(handle2);
    }
    
    lt_dlexit();
    return EXIT_SUCCESS;
}
