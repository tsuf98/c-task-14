#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objects.h"

void translate_instruction(FILE *fileptr, Instruction_obj *object);
void translate_data(FILE *fileptr, Instruction_obj *data);
int dec_to_special_binary_base(FILE *fileptr, int number, int length);

void translate_to_binary(char *ob_filename, Instruction_obj *instructions, Data_obj *data)
{
    FILE *target;
    int i, data_amount, instructions_amount, address;
    target = fopen(ob_filename, "w");
    data_amount = instructions_amount = 0;
    address = ADDRESS_STARTS_AT;

    for (i = 0; data[i].exists; i++)
    {
        data_amount += data[i].lines_no;
    }

    for (i = 0; instructions[i].exists; i++)
    {
        instructions_amount += instructions[i].lines_no;
    }

    fprintf(target, "%d %d\n", instructions_amount, data_amount);

    for (i = 0; instructions[i].exists; i++)
    {
        translate_instruction(target, instructions[i]);
    }

    for (i = 0; data[i].exists; i++)
    {
        translate_data(target, data[i]);
    }

    fclose(target);
}

int dec_to_special_binary_base(FILE *fileptr, int number, int length)
{
    for (int i = (length - 1); i >= 0; i--)
    {
        int k;
        k = n >> i;
        if (k & 1)
        {
            fputs(fileptr, ONE);
        }
        else
        {
            fputs(fileptr, ZERO);
        }
    }
}

void translate_instruction(FILE *fileptr, Instruction_obj *object)
{
}

void translate_data(FILE *fileptr, Instruction_obj *data)
{
}