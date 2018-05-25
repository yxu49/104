#include <bitset>
#include <iostream>
#include <string>
#include <unordered_map>

#include "symboltable.h"
#include "astree.h"
#include "lyutils.h"
#include "string_set.h"

vector<symbol_table*> symbol_stack;
vector<int> block_count;
int next_block = 1;
FILE* fsym;

void setsymfile(FILE* out) {
   fsym = out;
}

symbol* newsymbol(astree* node){
   if(symbol_stack.empty() )
   {
      symbol_stack.push_back(new symbol_table);
      symbol_stack.push_back(nullptr);
   }
   if(block_count.empty() )
   {
      block_count.push_back(0);
   }
   symbol* newsym = new symbol();
   newsym->attributes = node->attributes;
   newsym->fields = nullptr;
   newsym->blocknr = block_count.back();
   newsym->filenr = node->lloc.filenr;
   newsym->linenr = node->lloc.linenr;
   newsym->offset = node->lloc.offset;
   newsym->parameters = nullptr;
   return newsym;
}


const char* check_attr(astree* node)
{
   //cout<<node->lexinfo->c_str();
   string out = "";
    if (node->attributes[ATTR_field]) 
        out += "field ";
    if (node->attributes[ATTR_void]) 
        out += "void ";
    if (node->attributes[ATTR_int]) 
        out += "int ";
    if (node->attributes[ATTR_null])
        out += "null ";
    if (node->attributes[ATTR_string])
        out += "string ";
    if (node->attributes[ATTR_struct])
        out += "struct ";
    if (node->attributes[ATTR_array])
        out += "array ";
    if (node->attributes[ATTR_function])
        out += "function ";
    if (node->attributes[ATTR_variable])
        out += "variable ";
    if (node->attributes[ATTR_typeid]) 
        out += "typeid ";
    if (node->attributes[ATTR_param]) 
        out += "param ";
    if (node->attributes[ATTR_lval]) 
        out += "lval ";
    if (node->attributes[ATTR_const]) 
        out += "const ";
    if (node->attributes[ATTR_vreg])
        out += "vreg ";
    if (node->attributes[ATTR_vaddr])
        out += "vaddr ";
   //cout<<out.c_str();
   return out.c_str();
}

void setattributes(astree* node){
   //cout<<"I got here";
   switch(node->symbol)
   {
      case TOK_VOID:       node->attributes[ATTR_void]=1;
                           if (node->children[0]!=nullptr)
                           node->children[0]-> 
                              attributes[ATTR_void]=1;
                           break;
      case TOK_INT:        node->attributes[ATTR_int]=1; 
                           node->attributes[ATTR_lval]=1; 
                           if (node->children[0]!=nullptr)
                           node->children[0]-> 
                                 attributes[ATTR_int]=1;
                           break;
      case TOK_NULL:       node->attributes[ATTR_null] = true;
                           node->attributes[ATTR_const] = true;
                           break;
      case TOK_STRING:     node->attributes[ATTR_string] = true;
                           node->attributes[ATTR_lval] = true;
                           if (node->children.size() > 0)
                           node->children[0]-> 
                                attributes[ATTR_string] = true;
                           break;
      case TOK_STRUCT:     node->attributes[ATTR_struct] = true;
                           node->attributes[ATTR_typeid] = true;
                           if(node->children[0]!=nullptr)
                           {
                             node->children[0]->
                             attributes[ATTR_struct]=1;
                             node->children[0]->
                             attributes[ATTR_variable]=0;
                             node->children[0]->
                             attributes[ATTR_lval]=0;
                           }
                           node->attributes[ATTR_variable] = false;
                           node->attributes[ATTR_lval] = false;
                           break;
      case TOK_ARRAY:      node->attributes[ATTR_array] = true;
                           //check_array(node);
                           break;
      
      case TOK_FUNCTION:   node->attributes[ATTR_function] = true;
                        node->children[0]->children[0]->
                           attributes[ATTR_function] = true;
                           node->attributes[ATTR_variable] = false;
                           node->attributes[ATTR_lval] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_variable] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_lval] = false;
                           break;
      case TOK_PROTOTYPE:
                        node->children[0]->children[0]->
                           attributes[ATTR_variable] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_lval] = false;
                           break;
      case TOK_PARAMLIST:  for (size_t index = 0;
                                 index < node->children.size(); ++index)
                        node->children[index]->children[0]->
                                 attributes[ATTR_param] = true;
                           break;
      case TOK_STRINGCON:  node->attributes[ATTR_string] = true;
                           node->attributes[ATTR_const] = true;
                           break;
      case TOK_CHARCON:    node->attributes[ATTR_int] = true;
                           node->attributes[ATTR_const] = true;  
                           break;
      case TOK_INTCON:     node->attributes[ATTR_int] = true;
                           node->attributes[ATTR_const] = true;
                           break;
      case TOK_FIELD:      node->attributes[ATTR_field] = true;
                           node->attributes[ATTR_struct] = true;
                           break;
      case TOK_TYPEID:     node->attributes[ATTR_typeid] = true;
                           for(size_t index = 0; 
                              index < node->children.size(); ++index)
                           {
                      node->children[index]->
                               attributes[ATTR_typeid] = true;
                      node->children[index]->
                              attributes[ATTR_struct] = true;
                      break;
                      }
      case TOK_IDENT:      node->attributes[ATTR_lval] = true;
                           node->attributes[ATTR_variable] = true;
                           break;
      case TOK_VARDECL:    node->attributes[ATTR_variable] = true;
                           break;
      case TOK_DECLID:     node->attributes[ATTR_lval] = true;
                           node->attributes[ATTR_variable] = true;
                           break;
      default:           
                           break;
   }
}

void insert_symbol (symbol_table table, const string* key, 
                     symbol* sym, astree* node) {
   table[key] = sym;
   for (size_t size = 1; size < block_count.size(); ++size) {
      fprintf(fsym, "   ");
   }
   fprintf (fsym, "%s (%zu.%zu.%zu) {%zu} %s\n", key->c_str(), 
            sym->filenr, sym->linenr, sym->offset, sym->blocknr, 
            check_attr (node));
}

void structsym (astree* root) 
{
   symbol* sym = newsymbol(root->children[0]);
   const string* key;
   symbol_table fields;
   
   sym->fields = &fields;
   key = root->children[0]->lexinfo;
   insert_symbol(*symbol_stack[0], key, sym, root->children[0]);
   for (size_t index = 1; index < root->children.size(); ++index) 
   {
     astree* field = root->children[index]->children[0];
     sym = newsymbol(field);
     key = field->lexinfo;
     fprintf (fsym, "   ");
     insert_symbol(fields, key, sym, field);
   }
}

void block (astree* root) 
{
      block_count.push_back(next_block);
      next_block++;
      symbol_stack[block_count.back()] = new symbol_table;
      symbol_stack.push_back(nullptr);
      traverse(root);
      block_count.pop_back();
}

void funcsym(astree* root){
   astree* function = root->children[0]->children[0];
   symbol* sym = newsymbol(function);
   const string* key;
   vector<symbol*> params;

   sym->parameters = &params;
   key = function->lexinfo;
   insert_symbol (*symbol_stack[0], key, sym, function);
   
   astree* paramlist = root->children[1];
   for (size_t index = 0; 
            index < paramlist->children.size(); ++index){
      astree* param = paramlist->children[index]->children[0];
      sym = newsymbol(param);
      key = param->lexinfo;
      ++sym->blocknr;
      params.push_back(sym);
      fprintf(fsym, "   ");
      insert_symbol (*symbol_stack[0], key, sym, param);
   }
   
   block(root->children[2]); 
}

void protosym(astree* root){
   astree* proto = root->children[0]->children[0];
   symbol* sym = newsymbol(proto);
   const string* key;
   vector<symbol*> params;

   sym->parameters = &params;
   key = proto->lexinfo;
   insert_symbol (*symbol_stack[0], key, sym, proto);
   
   astree* paramlist = root->children[1];
   for (size_t index = 0; 
         index < paramlist->children.size(); ++index){
      astree* param = paramlist->
         children[index]->children[0];
      sym = newsymbol(param);
      ++sym->blocknr;
      params.push_back(sym);
   }
}

void make_vardecl(astree* root){
   astree* vardecl = root->children[0]->children[0];
   symbol* sym = newsymbol(vardecl);
   const string* key=vardecl->lexinfo;
   insert_symbol (*symbol_stack[block_count.back()], 
                  key, sym, vardecl);
   
}

void runattributes(astree* root){
   //cout<<root->lexinfo->c_str()<<"\n";
   for( size_t index = 0; index < root->children.size(); ++index){
      runattributes(root->children[index]);
   }
   //cout<<"start set attribute"<<root->lexinfo->c_str()<<"\n";
   setattributes(root);
}

void traverse(astree* root){
   for (size_t index = 0; index < root->children.size(); ++index) {
      
      int nodesymbol = root->children[index]->symbol;
      switch(nodesymbol){
         case TOK_STRUCT:  
            structsym(root->children[index]);
            fprintf(fsym, "\n");
            break;
         case TOK_FUNCTION:
            funcsym(root->children[index]); 
            fprintf(fsym, "\n");
            break;
         case TOK_PROTOTYPE:
            protosym(root->children[index]); 
            fprintf(fsym, "\n");
            break;
         case TOK_VARDECL:
            make_vardecl(root->children[index]);
            break;
         case TOK_IF:
            block(root->children[index]->children[1]);
            break;
         case TOK_IFELSE:
            block(root->children[index]->children[1]);
            block(root->children[index]->children[2]);
            break;
         default:
            break;
      }
   }  
}

      
