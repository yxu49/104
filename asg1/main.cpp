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
            yydebug = 1;break;
        case'D':
            cpp_opt= optarg;
            Dflag=1;
            break;
        cpp
        }
    }
}