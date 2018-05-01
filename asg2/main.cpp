#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "string_set.h"
#include "auxlib.h"
#include "astree.h"
#include "lyutils.h"
#define YYEOF 0
#define BUFSIZE 4096 
using namespace std;

char* replaceext(char* orig,string newext)
//return char pointer with newext
{
  char newfilename[strlen(orig)+2];
  strcpy(newfilename,orig);
  *strrchr(newfilename,'.')='\0';
  strcat(newfilename,newext.c_str());
  return strdup(newfilename);
}
char* addparen(char* noparen)
{
  char newstr[strlen(noparen)+3];
  newstr[0]='(';
  newstr[1]='\0';
  strcat(newstr,noparen);
  newstr[strlen(noparen)+1]=')';
  newstr[strlen(noparen)+2]='\0';
  return strdup(newstr);
}


int main(int argc,char** argv)
{
 // exec::execname=basename(argv[0]);
  int opt=0,Dflag=0;
  char *cvalue=NULL;
  //start argument processing
  while((opt=getopt(argc,argv,"lyD:@:"))!=-1)
  switch(opt)
  {
  case'l':
    yy_flex_debug=1;
    break;
  case'y':
    yydebug=1;
    break;
  case'D':
    cvalue=optarg;
    Dflag=1;
    break;
  case'@':
    set_debugflags(optarg);
    break;
  case'?':
    if(optopt=='D')
      fprintf(stderr,"Option -D require argument");
    else if(optopt=='@')
      fprintf(stderr,"Option -@ require argument");
    else if(isprint(optopt))
      fprintf(stderr,"Unknown option '-%c' .\n",optopt);
    else
      fprintf(stderr,"Unknown option character '\\x%x' .\n",optopt);
    return 1;
  default:
    abort();
  }
  if(optind==argc)//catch for no input file
  {
    fprintf(stderr,"no input file is passed\n");
    exit(1);
  }
  char* inputpath=argv[optind];
  char* inputfile=basename(argv[optind]);
  if(strcmp(".oc",inputfile+strlen(inputfile)-3)!=0)
    fprintf(stderr,"file extension mismatched\n");
  ifstream fin(inputpath);
  if(!fin)
  {
    fprintf(stderr,"such file doesn't exist\n");
    exit(1);
  }
  char prepro[100]="/usr/bin/cpp ";
  if(Dflag==1)//cpp argument flag
  {
    strcat(prepro,"-D");
    strcat(prepro,cvalue);
    strcat(prepro," ");
    strcat(prepro,inputpath);
  }
  else
    strcat(prepro,inputpath);
  //---------------output process begin------------------------
  char *tokfilename=replaceext(inputfile,".tok");
  char *strfilename=replaceext(inputfile,".str");
  FILE *stroutput=fopen(strfilename,"w");
  //FILE *tokoutput=fopen(tokfilename,"w");
  ofstream tokoutput(tokfilename);
  yyin=popen(prepro,"r");
  int lexnum=1;
  for(;;)
  {
    if(lexnum==YYEOF)
      break;
    lexnum=yylex();
    string_set::intern(yytext);
    tokoutput<<right<<setfill(' ')<<setw(4)<<lexer::lloc.linenr;
    tokoutput<<setw(4)<<lexer::lloc.filenr;
    tokoutput<<"."<<setfill('0')<<setw(3)<<lexer::lloc.offset;
    tokoutput<<setfill(' ')<<setw(4)<<lexnum;
    tokoutput<<" "<<setw(16)<<left<<parser::get_tname(lexnum);
    char* ytext=addparen(yytext);
    tokoutput<<" "<<setw(16)<<ytext<<endl;
    delete ytext;
  }
  string_set::dump(stroutput);
  fclose(yyin);
  fclose(stroutput);
  tokoutput.close();
  free(tokfilename);
  free(strfilename);
 /* 
  while(lexnum!=0)
  {
    lexnum=yylex();
    fprintf(outputfile,"  %zu %zu.%zu %i %s (%s)\n",lexer::lloc.linenr,
lexer::lloc.filenr, lexer::lloc.offset,lexnum,parser::get_tname(lexnum),
yytext);
    string_set::intern(yytext);
  }
  string_set::dump(outputfile);
   
  fclose(yyin);
   
  fclose(outputfile);
  fclose(cpreprocess);
  free(outputfilename);
  */
  return EXIT_SUCCESS;

}
