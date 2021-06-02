#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int main(int argc, char** argv) {
    void *handle;
    void *handle2;
    void (*formula_printf)(const char*);

    if (argc !=2 || argc != 3) {
        fprintf(stderr, "Improper number of arguments.\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "HOCl") == 0) {
        handle = dlopen("./chlorine.so", RTLD_NOW);
    } else if (strcmp(argv[1], "HOBr") == 0) {
        handle = dlopen("./bromine.so", RTLD_NOW);
    } else if (strcmp(argv[1], "HOCl") == 0 
               && strcmp(argv[2], "HOBr") == 0) {
        handle = dlopen("./chlorine.so", RTLD_NOW);
        handle2 = dlopen("./bromine.so", RTLD_NOW);
    } else if (strcmp(argv[1], "HOBr") == 0 
               && strcmp(argv[2], "HOCl") == 0) {
        handle = dlopen("./bromine.so", RTLD_NOW);
        handle2 = dlopen("./chlorine.so", RTLD_NOW);
    } else {
        fprintf(stderr, "Error: unknown formula: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (!handle || (argc == 3 && !handle && !handle2)) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    *(void**)(&formula_printf) = dlsym(handle, "print_name");
    if (!formula_printf) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    formula_printf(argv[1]);

    if (argc == 3) {
        *(void**)(&formula_printf) = dlsym(handle2, "print_name");

        if (!formula_printf) {
            /* no such symbol */
            fprintf(stderr, "Error: %s\n", dlerror());
            dlclose(handle2);
            return EXIT_FAILURE;
        }

        formula_printf(argv[2]);

    }

    dlclose(handle);
    if (argc == 3) {
        dlclose(handle2);
    }

    return EXIT_SUCCESS;
}
