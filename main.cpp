#include <stdio.h>
#include <stdlib.h>

#include "extern.h"

int status = EXIT_SUCCESS;

void yyerror (const char* error) {
   printf ("%s\n", error);
   status = EXIT_FAILURE;
}

int main () {
   yyparse();
   return status;
}