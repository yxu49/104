#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__


#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

#include "auxlib.h"
#include "astree.h"


enum { ATTR_void, ATTR_int, ATTR_null,
       ATTR_string, ATTR_struct, ATTR_array, ATTR_function,
       ATTR_variable, ATTR_field, ATTR_typeid, ATTR_param,
       ATTR_lval, ATTR_const, ATTR_vreg, ATTR_vaddr,
       ATTR_bitset_size,
};
using attr_bitset = bitset<ATTR_bitset_size>;

struct symbol;
struct astree;
using symbol_table = unordered_map<const string*,symbol*>;
using symbol_entry = pair<string*,symbol*>;

struct symbol {
   attr_bitset attributes;
   symbol_table* fields;
   size_t filenr, linenr, offset;
   size_t blocknr;
   vector<symbol*>* parameters;
};

void setsymfile(FILE* out);
const char* check_attr(astree* node);
void setattrbutes(astree* node);
void structsym(astree* root);
void block(astree* root);
void funcsym(astree* root);
void protosym(astree* root);
void runattributes(astree* root);
void traverse(astree* root);


#endif
