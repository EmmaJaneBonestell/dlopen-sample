#include "chemical.h"

// All non-local symbols that will be exported must be renamed if they will conflict. 
// Rename them by the name of the source file _LTX_ name of the symbol

#define chemical_name chlorine_LTX_chemical_name

void chemical_name (const char *formula) {
    printf("The IUPAC name of %s is hypochlorous acid.\n", formula);
}
