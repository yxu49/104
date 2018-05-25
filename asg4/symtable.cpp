#include "symtable.h"
#include "astree.h"
symbol* new_symbol(astree* node)
{
  symbol* sym=new symbol();
  sym->attr=node->attributes;
  sym->fields=nullptr;
  sym->lloc = node->lloc;
  sym->block_nr=node->block_nr;
  return sym;
}
void insert_symbol(symbol_table* symtab,astree* node)
{
  symbol* sym=new_symbol(node);
  symbol_entry entry=symbol_entry(const_cast<string*>(node->lexinfo),sym);
}
symbol* search_symbol(symbol_table* symtab,astree* node)
{
  string* lexinfo = (string *)node->lexinfo;
  if(!symtab->count(lexinfo))
    return nullptr;
  symbol* sym=new_symbol(node);
  symbol_entry entry=symbol_entry(const_cast<string*>(node->lexinfo),sym);
  return entry.second;
}
