#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ltdl.h>

// The following preprocessor define is not necessary, 
// and it will cause a redefinition warning. However, while
// I am not 100% certain, but I believe that if you were to link two
// such files together, the symbol would conflict, so it would
// probably be good practice to rename it to the source file's name
// or an otherwise unique identifier.

// #define lt_preloaded_symbols lt_main_LTX_preloaded_symbols


// You must include ltdl.h; if you don't, you may see various errors
// such as lt__PROGRAM__LTX_preloaded_symbols being undefined, which is
// the default value of lt_preloaded_symbols.

int main(int argc, char *argv[]) {
    lt_dlhandle handle;
    lt_dlhandle handle2;
    void (*formula_printf)(const char*);

//  You must declare an extern int X() for each symbol you will use.
//  Not 100% certain. but fairly sure they will always be an int
//  regardless of actual type.

    extern int bromine_LTX_chemical_name();
    extern int chlorine_LTX_chemical_name();

//  The array of structures is in the format of const char *, void *addr.
//  It will begin with "@PROGRAM@", void * 0 and end with 0, void * 0.
//  All strings must be null terminated. Inbetween, you will declare
//  the source file name, followed by all the symbols from it you wish
//  to use. The symbols will be void cast to their own address. 

    const lt_dlsymlist lt_preloaded_symbols[] ={ 
        {"@PROGRAM@", (void *) 0},
        {"chlorine.a", (void *) 0},
//  Note that the source file is the basename chlorine with the extension
//  ".a" -- this will always be the case.
        {"chlorine_LTX_chemical_name", (void *) &chlorine_LTX_chemical_name},
//  If you had more symnbols from chlorine.c, they would go before bromine.
        {"bromine.a", (void *) 0},
        {"bromine_LTX_chemical_name", (void *) &bromine_LTX_chemical_name},
        {0, (void *) 0}
    };

    LTDL_SET_PRELOADED_SYMBOLS();

    lt_dlinit();

    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Improper number of arguments : %s .\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "HOCl") == 0) {    
       handle = lt_dlopen("chlorine.so");
//  The file extension for a preopened object does not actually matter 
//  for the lt_dlopen function.
        if (argc == 3 && strcmp(argv[2], "HOBr") == 0) {
            handle2 = lt_dlopen("bromine.so");
        }
    } else if (strcmp(argv[1], "HOBr") == 0) {
        handle = lt_dlopen("bromine.so");
        if (argc == 3 && strcmp(argv[2], "HOCl") == 0) {
            handle2 = lt_dlopen("chlorine.so");
        }
    } else {
        fprintf(stderr, "Error: unknown formula: %s\n", argv[1]);
        return EXIT_FAILURE;
    } 

    if (!handle) {
        printf("Handle load lib failed.\n");
        // fail to load the library
        fprintf(stderr, "Error: %s\n", lt_dlerror());
        return EXIT_FAILURE;
    }

    *(void**)(&formula_printf) = (void *)lt_dlsym(handle, "chemical_name");
    if (!formula_printf) {
        // no such symbol
        printf("No such symbol. \n"); 
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
        }

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
