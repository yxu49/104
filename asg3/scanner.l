/* $Id: lexer.l,v 1.7 2016-10-27 19:45:59-07 - - $ */

%{

#include "lyutils.h"

#define YY_USER_ACTION  { lexer::advance(); }

// int yylval_token(int symbol)
// {
//     yylval = new astree(symbol, lexer::lloc, yytext);
//     string_set::intern(yytext);
//     fprintf(tokoutput, "%4lu%4lu.%03lu %4i %-16s (%s)\n",
//             lexer::lloc.linenr, lexer::lloc.filenr, lexer::lloc.linenr,
//             symbol, parser::get_tname(symbol), yytext);
//     return symbol;
// }

%}

%option 8bit
%option debug
%option nodefault
%option noinput
%option nounput
%option noyywrap
%option warn
/*%option verbose*/

LETTER          [A-Za-z_]
DIGIT           [0-9]
INTEGER         {DIGIT}+
DIFSHI          ("[]")
CHAR            ('([^\\'\n]|\\[\\'"0nt])')
STRING          (\"([^\\"\n]|\\[||'"0nt])*\")
MANTISSA        ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)
EXPONENT        ([Ee][+-]?{DIGIT}+)
NOTNUMBER       ({MANTISSA}[Ee][+_]?)
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
%%

"#".*           { lexer::include(); }
[ \t]+          { }
\n              { lexer::newline(); }

"+"             { return yylval_token ('+'); }
"-"             { return yylval_token ('-'); }
"*"             { return yylval_token ('*'); }
"/"             { return yylval_token ('/'); }
"%"             { return yylval_token ('%'); } 
"!"             { return yylval_token ('!'); }
"="             { return yylval_token ('='); } 
"=="            { return yylval_token (TOK_EQ); }
"!="            { return yylval_token (TOK_NE); }
"<"             { return yylval_token (TOK_LT); }
"<="            { return yylval_token (TOK_LE); }
">"             { return yylval_token (TOK_GT); }
">="            { return yylval_token (TOK_GE); }
"^"             { return yylval_token ('^'); }
"("             { return yylval_token ('('); }
")"             { return yylval_token (')'); }
";"             { return yylval_token (';'); }
"["             { return yylval_token ('['); }
"]"             { return yylval_token (']'); }
"{"             { return yylval_token ('{'); }
"}"             { return yylval_token ('}'); }
","             { return yylval_token (','); }
"."             { return yylval_token ('.'); }
"[]"            { return yylval_token (TOK_NEWARRAY);}
"if"            { return yylval_token (TOK_IF); }
"else"          { return yylval_token (TOK_ELSE); }
"while"         { return yylval_token (TOK_WHILE); }
"return"        { return yylval_token (TOK_RETURN); }
"int"           { return yylval_token (TOK_INT); }
"string"        { return yylval_token (TOK_STRING); }
"struct"        { return yylval_token (TOK_STRUCT); }
"void"          { return yylval_token (TOK_VOID); }
"new"           { return yylval_token (TOK_NEW); }
"null"          { return yylval_token (TOK_NULL); }
"not"           { return yylval_token (TOK_NOT);}
{IDENT}         { return yylval_token (TOK_IDENT); }
{INTEGER}       { return yylval_token (TOK_INTCON); }
{CHAR}          { return yylval_token (TOK_CHR); }
{STRING}        { return yylval_token (TOK_STRINGCON); }

.               { lexer::badchar (*yytext); }

%%

