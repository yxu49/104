#ifndef _TYPECHECK_H
#define _TYPECHECK_H
#include "symstack.h"
void typecheck(FILE* symfile,astree* node, symstack* symbol_stack, symbol_table* symbol_table);
#endif
