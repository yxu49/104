#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__
#include "astree.h"
#include "lyutils.h"
using namespace std;
//struct symbol;
//using symbol_table=unordered_map<const string*, symbol*>;
//using symbol_entry=pair<const string*, symbol*>;
struct symbol
{
  attr_bitset attr;
  symbol_table* fields;
  location lloc;
  size_t block_nr;
  vector<symbol*>* parameters;
};
symbol* new_symbol(astree* node);
void insert_symbol(symbol_table* symbtab,astree* node);
symbol* search_symbol(symbol_table* symtab,astree* node);
symbol* search_type_name(symbol_table* symtab,astree);

#endif
