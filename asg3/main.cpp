//main.cpp
//Tian Qiu(tqiu1) and yijiong xu (yxu49)
#include<string>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<vector>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "auxlib.h"
#include "astree.h"
#include "lyutils.h"
#include "string_set.h"
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


int main(int argc,char** argv)
{
  yy_flex_debug=0;
  yydebug=0;
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
  exec::execname=inputfile;
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
  char *tokfilename=replaceext(inputfile,".tok");
  char *strfilename=replaceext(inputfile,".str");
  char *astfilename=replaceext(inputfile,".ast");
  FILE* stroutput=fopen(strfilename,"w");
  tokoutput=fopen(tokfilename,"w");
  FILE* astoutput=fopen(astfilename,"w");
  yyin=popen(prepro,"r");
  yyparse();
  cout<<yyparse_astree->lexinfo->c_str(); 
  string_set::dump(stroutput);
  astree::print(astoutput,yyparse_astree);
  fclose(yyin);
  fclose(tokoutput);
  fclose(stroutput);
  fclose(astoutput);
  free(tokfilename);
  free(strfilename);
  free(astfilename);
  return EXIT_SUCCESS;

}
