#ifndef _SYMSTACK_H
#define _SYMSTACK_H
#include "astree.h"
#include "symtable.h"

extern size_t next_block;

struct symstack
{
  vector <symbol_table*> symbol_stack;
  void enter_block();
  void leave_block();
  void define_ident(astree* node);
  symbol* search_ident(astree* node);
};
#endif
