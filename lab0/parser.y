// $Id: parser.y,v 1.1 2018-04-02 15:31:00-07 - - $

%{

#include <stdio.h>
#include <stdlib.h>

#include "extern.h"

%}

%token NUMBER

%%

input : input expr '\n' { printf ("%.15g\n", $2); }
      | '\n'            { printf ("\n"); }
      | error '\n'      { printf ("error\n"); }
      |
      ;

expr  : NUMBER          { $$ = atof (yytext); }
      | expr expr '+'   { $$ = $1 + $2; }
      | expr expr '-'   { $$ = $1 - $2; }
      | expr expr '*'   { $$ = $1 * $2; }
      | expr expr '/'   { $$ = $1 / $2; }
      ;

%%

