#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "string_set.h"
#include "auxlib.h"
using namespace std;
char *replaceext(char *orig, char *newext)
//return char pointer with newext
{
  char *newstr = new char[strlen(orig) + 1];
  int dotind = -1;
  for (size_t i = 0; i < strlen(orig); i++)
  {
    if (orig[i] == '.')
    {
      dotind = i;
      break;
    }
  }
  strncpy(newstr, orig, dotind);
  strcat(newstr, newext);
  return newstr;
}
int main(int argc, char **argv)
{
  int opt = 0, yy_flex_debug = 0, yydebug = 0, Dflag = 0;
  char *cvalue = NULL;
  //start argument processing
  while ((opt = getopt(argc, argv, "lyD:@:")) != -1)
    switch (opt)
    {
    case 'l':
      yy_flex_debug = 1;
      break;
    case 'y':
      yydebug = 1;
      break;
    case 'D':
      cvalue = optarg;
      Dflag = 1;
      break;
    case '@':
      set_debugflags(optarg);
      break;
    case '?':
      if ( == 'D')
        fprintf(stderr, "Option -D require argument");
      else if (optopt == '@')
        fprintf(stderr, "Option -@ require argument");
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option '-%c' .\n", optopt);
      else
        fprintf(stderr, "Unknown option character '\\x%x' .\n", optopt);
      return 1;
    default:
      abort();
    }
  if (optind == argc) //catch for no input file
  {
    fprintf(stderr, "no input file is passed\n");
    exit(1);
  }
  char *inputpath = argv[optind];
  char *inputfile = basename(argv[optind]);
  if (strcmp(".oc", inputfile + strlen(inputfile) - 3) != 0)
    fprintf(stderr, "file extension mismatched\n");
  ifstream fin(inputpath);
  if (!fin)
  { 
    fprintf(stderr, "such file doesn't exist\n");
    exit(1);
  }
  char prepro[100] = "/usr/bin/cpp ";
  if (Dflag == 1) //cpp argument flag
  {
    strcat(prepro, "-D");
    strcat(prepro, cvalue);
    strcat(prepro, " ");
    strcat(prepro, inputpath);
  }
  else
    strcat(prepro, inputpath);
  char *outputfilename = replaceext(inputfile, (char *)".str");
  FILE *outputfile = fopen(outputfilename, "w"); //create output file
  FILE *cpreprocess = popen(prepro, "r");
  char str[100];
  while (fgets(str, 100, cpreprocess) != NULL) //getting lines from cpp output
  {
    char *token;
    char *rest = str;
    while ((token = strtok_r(rest, " \n\t", &rest)))
      string_set::intern(token);
  }
  string_set::dump(outputfile); //dump cpp output into outputfile
  fclose(outputfile);
  fclose(cpreprocess);
  delete (outputfilename);
  return EXIT_SUCCESS;
}
