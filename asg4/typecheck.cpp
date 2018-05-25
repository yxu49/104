#include "typecheck.h"
#include "lyutils.h"
astree* current_struct=nullptr;
astree* current_field=nullptr;

void printhelper(FILE* symfile,astree* node)
{
  astree* left=nullptr;
  if(node->children.size()>=1)
  {
    left=node->children[0];
    fprintf(symfile,"%s(%zd.%zd.$zd) ",
    left->lexinfo->c_str(),node->lloc.filenr,node->lloc.linenr,
    node->lloc.offset);
  }
  else
  {
    fprintf(symfile,"%s (%zd.%zd.%zd) ",
    node->lexinfo->c_str(),node->lloc.filenr,node->lloc.linenr,
    node->lloc.offset,node->block_nr);
  }
  if(node->attributes[ATTR_struct])
    fprintf(symfile,"struct \"%s\" ",current_struct->lexinfo->c_str());
  if(node->attributes[ATTR_field])
    fprintf(symfile,"field {%s} ",current_field->lexinfo->c_str());
  if(node->attributes[ATTR_function])
    fprintf(symfile,"function ");
  if(node->attributes[ATTR_void])
    fprintf(symfile,"function ");
  if(node->attributes[ATTR_int])  
    fprintf(symfile,"int ");
  if(node->attributes[ATTR_null])
    fprintf(symfile,"null ");
  if(node->attributes[ATTR_string])
    fprintf(symfile,"string ");
  if(node->attributes[ATTR_const])
    fprintf(symfile,"const ");
  if(node->attributes[ATTR_array])
    fprintf(symfile,"array ");
  if(node->attributes[ATTR_typeid])
    fprintf(symfile,"type_id ");
  if(node->attributes[ATTR_vreg])
    fprintf(symfile,"vreg ");
  if(node->attributes[ATTR_vaddr])
    fprintf(symfile,"vaddr ");
  if(node->attributes[ATTR_variable])
    fprintf(symfile,"variable ");
  if(node->attributes[ATTR_lval])
    fprintf(symfile,"lval ");
  if(node->attributes[ATTR_param])
    fprintf(symfile,"param");
  fprintf(symfile,"\n");
}

bool primcheck(astree* left,astree* right)
{
  for(size_t i=0;i<ATTR_function;i++)
    if(left->attributes[i]==1&&right->attributes[i]==1)
      return true;
  return false;
}

void typecheck_function(FILE* symfile,astree* node,symstack* symbol_stack,symbol_table* symbol_table)
{
  cout<<node->lexinfo->c_str()<<" "<<parser::get_tname(node->symbol)<<"\n";
  astree* left=nullptr;
  astree* right=nullptr;
  if(node->children.size()>=1)
    left=node->children[0];
  if(node->children.size()>=2)
    right=node->children[1];
  symbol* s;
  switch(node->symbol)
  {
    case TOK_ROOT: break;
    case '(':      break;
    case ')':      break;
    case '{':      break;
    case '}':      break;
    case '[':      break;
    case ']':      break;
    case ';':      break;

    case TOK_RETURN:       break;
    case TOK_RETURNVOID:   break;
    case TOK_PARAMLIST:    break;
    case TOK_NEW:
    {
      for(size_t i=0;i<ATTR_bitset_size;i++)
        if(left->attributes[i])
	  node->attributes[i]=1;
    }
    break;
    case TOK_TYPEID:
    {
      node->attributes[ATTR_typeid]=1;
      break;
    }
    case TOK_DECLID:
    {
      symbol_stack->define_ident(node);
      break;
    }
    case TOK_FIELD:
    {
      node->attributes[ATTR_field]=1;
      if(left!=nullptr)
      {
        left->attributes[ATTR_field]=1;
	for(size_t i=0;i<ATTR_function;i++)
          if(left->attributes[i])
	    node->attributes[i]=1;
      }
      break;
    }
    case TOK_INT:
    {
      if(left!=nullptr)
      {
        left->attributes[ATTR_int]=1;
        for(size_t i=0;i<ATTR_function;i++)
	 if(left->attributes[i])
	   node->attributes[i]=1;
      }
      break;
    }
    case TOK_PROTOTYPE:
    {
      left=node->children[0];
      astree* left2=left->children[0];
      current_field=left;
      node->attributes[ATTR_function]=1;
      left->attributes[ATTR_function]=1;
      left2->attributes[ATTR_function]=1;
      printhelper(symfile,left2);
      astree* middle=nullptr;
      node->children[1]->children[0];
      if(node->children[1]->children.size()>=1)
      {
        middle=node->children[1]->children[0];
	middle->block_nr=middle->block_nr+1;
      }
      else
        middle=nullptr;
      
      while(middle!=nullptr)
      {
        left=middle->children[0];
	left->attributes[ATTR_param]=1;
	left->attributes[ATTR_variable]=1;
        left->attributes[ATTR_lval]=1;
	fprintf(symfile,"\t");
	left->block_nr=middle->block_nr;
	printhelper(symfile,left);
        if(middle->children.size()>=2)
        {
          middle=middle->children[1];
          middle->block_nr=left->block_nr;
        }
	else
	  middle=nullptr;
      }
      break;
    }
    case TOK_CALL:
    {
      node->attributes[ATTR_function]=1;
      break;
    }
    case TOK_CHAR: break;
    case TOK_VOID:
    {
      left-> attributes[ATTR_void]=1;
      break;
    }
    case TOK_STRING:
    {
      if(left!=nullptr)
      {
        left->attributes[ATTR_string]=1;
	for(size_t i=0;i<ATTR_function;i++)
	  if(left->attributes[i])
	    node->attributes[i]=1;
      }
      break;
    }
    case TOK_ARRAY:
    {
      left->attributes[ATTR_array]=1;
      if(left->children.size()>=1)
        left->children[0]->attributes[ATTR_array]=1;
      break;
    }
    case TOK_NEWARRAY:
    {
      left=node->children[0];
      for(size_t i=0;i<ATTR_function;i++)
        if(left->attributes[i])
	  node->attributes[i]=1;
      node->attributes[ATTR_array]=1;
      node->attributes[ATTR_vreg]=1;
      break;
    }
    case TOK_INDEX:
    {
      node->attributes[ATTR_vaddr]=1;
      node->attributes[ATTR_lval]=1;
    }
    case TOK_IDENT:
    {
      s=symbol_stack->search_ident(node);
      if(s==nullptr)
        s=search_symbol(symbol_table,node);
      if(s==nullptr)
        errprintf("Undefined identifier \n");
      else
        node->attributes=s->attr;
      break;
    }
    case TOK_STRUCT:
    {
      current_struct=left;
      current_field=left;
      node->attributes[ATTR_struct]=1;
      left->attributes[ATTR_struct]=1;
      printhelper(symfile,left);
      insert_symbol(symbol_table,left);
      s=search_symbol(symbol_table,left);
      right=node->children[1];
      while(right!=nullptr)
      {
        astree* leftchild=nullptr;
	if(right->children.size()>=1)
	{
	  leftchild=right->children[0];
	  fprintf(symfile,"\t");
	  printhelper(symfile,leftchild);
	}
	if(right->children.size()>=2)
	  right=right->children[1];
	else 
	  right=nullptr;
      }
      break;
    }
    case TOK_IF: break;
    case TOK_IFELSE: break;
    case TOK_WHILE: break;
    case '+':
    case '-':
    {
      node->attributes[ATTR_int]=1;
      node->attributes[ATTR_vreg]=1;
      if(left==nullptr) break;
      else if(right==nullptr)
      {
        if(left->attributes[ATTR_int]==0)
	  fprintf(stderr,"ERROR: right node no int attribute \n");
        else
	{
	  node->attributes[ATTR_vreg]=1;
	  node->attributes[ATTR_int]=1;
	}
      }
      else
      {
        if(left->attributes[ATTR_int]==0||right->attributes[ATTR_int]==0)
	{
          fprintf(stderr,"ERROR: left or right attr");
	  fprintf(stderr,"ibute is not int type \n");
        }
      }
      break;
    }
    case '*':
    case '/':
    case '%':
    {
      node->attributes[ATTR_int]=1;
      node->attributes[ATTR_vreg]=1;
      if(right==nullptr||left==nullptr)
        fprintf(stderr,"ERROR: left or right doesn't exist");
      else
        if(left->attributes[ATTR_int]==0||right->attributes[ATTR_int]==0)
	  fprintf(stderr,"ERROR: left or right are not int attribute\n");
      break;
    }
    case '!':
    {
      if(left==nullptr)  break;
      else if(left->attributes[ATTR_int]==0)
        fprintf(stderr,"ERROR: left no int attribute");
      else
      {
        node->attributes[ATTR_vreg]=1;
	node->attributes[ATTR_int]=1;
      }
      break;
    }
    case '=':
    {
      left=node->children[0];
      right=node->children[1];
      break;
    }
    case TOK_EQ:
    case TOK_NE:
    {
      if(node->children.size()>=1)
        left=node->children[0];
      else
        left=nullptr;
      if(node->children.size()>=2)
        right=node->children[1];
      else
        right=nullptr;
      if(primcheck(left,right)==true)
        node->attributes[ATTR_vreg]=1;
      else
        fprintf(stderr,"types do not match \n");
    break;
    }
    case TOK_GT:
    case TOK_GE:
    case TOK_LT:
    case TOK_LE:
    {
      if(node->children.size()>=1)
        left=node->children[0];
      else
        left=nullptr;
      if(node->children.size()>=2)
        right=node->children[1];
      else
        fprintf(stderr,"Types do not match \n");
      break;
    }
    case TOK_INTCON:
    case TOK_CHARCON:
    {
      node->attributes[ATTR_int]=1;
      node->attributes[ATTR_const]=1;
      break;
    }
    case TOK_STRINGCON:
    {
      node->attributes[ATTR_string]=1;
      node->attributes[ATTR_const]=1;
      break;
    }
    case TOK_NULL:
    {
      node->attributes[ATTR_null]=1;
      node->attributes[ATTR_const]=1;
      break;
    }
    default:
    {
      fprintf(stderr,"TOK_ doesn't exist");
      break;
    }
  }
}

void typecheck(FILE* symfile,astree* parseroot,symstack* symbol_stack,symbol_table* symbol_table)
{
  for(astree* child:parseroot->children)
  {
    typecheck(symfile,child,symbol_stack,symbol_table);
  }
  typecheck_function(symfile,parseroot,symbol_stack,symbol_table);
}
