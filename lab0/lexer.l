/* $Id: lexer.l,v 1.1 2018-04-02 15:31:00-07 - - $ */

%{

#include "extern.h"

%}

%option noyywrap

MANTISSA    ([[:digit:]]+\.?[[:digit:]]*|\.[[:digit:]]+)
EXPONENT    ([Ee][+-]?[[:digit:]]+)
INFINITY    ([Ii][Nn][Ff][[:isalnum:]]*)
NAN         ([Nn][Aa][Nn][[:isalnum:]]*)
NUMBER      ({MANTISSA})({EXPONENT})?|INFINITY|NAN


%%

"#".*
[ \t]+
\n         { return '\n'; }
{NUMBER}   { return NUMBER; }
"+"        { return '+'; }
"-"        { return '-'; }
"*"        { return '*'; }
"/"        { return '/'; }

%%
