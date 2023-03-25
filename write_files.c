#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objects.h"

void save_internals(char *ent_filename, Data_obj *data)
{
    int is_opened, i;
    FILE *fileptr;
    is_opened = 0;

    for (i = 0; data[i].exists; i++)
    {
        if (data[i].scope == ENT_SCOPE)
        {
            if (!is_opened)
            {
                fileptr = fopen(ent_filename, "w");
                is_opened = 1;
            }
            fprintf(fileptr, "%s  %d\n", data[i].label, data[i].address);
        }
    }

    if (is_opened)
    {
        fclose(fileptr);
    }
}

void save_externals(char *ext_filename, Instruction_obj *instructions, Data_obj *data)
{
    int is_opened, i, j;
    FILE *fileptr;
    is_opened = 0;

    for (i = 0; data[i].exists; i++)
    {
        if (data[i].scope == EXT_SCOPE)
        {
            if (!is_opened)
            {
                fileptr = fopen(ext_filename, "w");
                is_opened = 1;
            }
            for (j = 0; instructions[j].exists; j++)
            {
                if (strcmp(instructions[j].jumping_label, data[i].label) == 0)
                {
                    fprintf(fileptr, "%s  %d\n", data[i].label, instructions[j].address + 1);
                }
                else if (instructions[j].source.type == label && instructions[j].source.value == i)
                {
                    fprintf(fileptr, "%s  %d\n", data[i].label, instructions[j].address + (strlen(instructions[j].jumping_label) ? 2 : 1));
                }
                else if (instructions[j].destination.type == label && instructions[j].destination.value == i)
                {
                    fprintf(fileptr, "%s  %d\n",
                            data[i].label,
                            instructions[j].address +
                                (strlen(instructions[j].jumping_label)  ? 3 : instructions[j].source.type != null ? 2 : 1));
                }
            }
        }
    }

    if (is_opened)
    {
        fclose(fileptr);
    }
}