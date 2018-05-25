#include "astree.h"
#include "symtable.h"
#include "symstack.h"
void symstack::enter_block()
{
  next_block++;
  symbol_stack.push_back(nullptr);
}
void symstack::leave_block()
{
  symbol_stack.pop_back();
}
void symstack::define_ident(astree* node)
{
  if(symbol_stack.back()==nullptr)
  {
    symbol_stack.back()=new symbol_table;
  }
  insert_symbol(symbol_stack.back(),node);
}
symbol* symstack::search_ident(astree* node)
{
  for(auto symbol_table:symbol_stack)
  {
    if(symbol_table==nullptr)
      return nullptr;
    else
    {
      symbol* s=search_symbol(symbol_table,node);
      if(s!=nullptr)
        return s;
    }
  }
  return nullptr;
}


