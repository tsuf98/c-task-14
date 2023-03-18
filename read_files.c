#include "compile.h"

void read_files_and_compile(int argc, char **argv)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        compile(argv[i]);
    }
}
