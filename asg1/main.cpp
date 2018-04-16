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

char *replaceext(char *orig, char *newext)
//return char pointer with newext
{
    char *newstr = new char[strlen(orig) + 1];
    std::string origstr = std::string(orig);
    size_t dotind = origstr.find(".", 0);
    strncpy(newstr, orig, dotind);
    strcat(newstr, newext);
    return newstr;
}
int main(int argc, char **argv)
{
    int opt = 0, yy_flex_debug = 0, yydebug = 0, Dflag = 0;
    char *cpp_opt;
    opt = getopt(argc, argv, "lyD:@:");
    while (opt != -1)
    {
        switch (opt)
        {
        case '@':
            set_debugflags(optarg);
            break;
        case 'l':
            yy_flex_debug = 1;
            break;
        case 'y':
            yydebug = 1;
            break;
        case 'D':
            cpp_opt = optarg;
            Dflag = 1;
            break;
        default:
            fprintf(stderr, "bad option '-%c' .\n", optopt);
            break;
        }
    }
    if (optind == argc) //catch for no input file
    {
        fprintf(stderr, "no input file is passed\n");
        exit(1);
    }
    (void)yy_flex_debug; 
    (void)yydebug;
    char *inputpath = argv[optind];
    char *inputfile = basename(argv[optind]);
    std::string inputfilestr = std::string(inputfile);
    static const size_t npos = -1;
    if (inputfilestr.find(".oc", 0) == npos)
        fprintf(stderr, "file extension mismatched\n");
    ifstream fin(inputpath);
    if (!fin)
    {
        fprintf(stderr, "such file doesn't exist\n");
        exit(1);
    }
    string prepro = "/usr/bin/cpp ";
    if (Dflag == 1) //cpp argument flag
    {
        prepro = prepro + "-D" + cpp_opt + " " + inputpath;
    }
    else
        prepro = prepro + inputpath;
    const char *pp = prepro.c_str();
    char *outputfilename = replaceext(inputfile, (char *)".str");
    FILE *outputfile = fopen(outputfilename, "w"); //create output file
    FILE *cpreprocess = popen(pp, "r");
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