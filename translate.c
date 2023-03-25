#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "objects.h"

void translate_instruction(FILE *fileptr, Instruction_obj *object, Data_obj *data, int *address);
void translate_data(FILE *fileptr, Data_obj *data, int *address);
void dec_to_special_binary_base(FILE *fileptr, int number, int length);
int find_jumping_label_address(Instruction_obj *instructions, char *jumping_label);
int find_instruction_label_address(Instruction_obj *instructions, char *label, int default_val);

void translate_to_binary(char *ob_filename, Instruction_obj *instructions, Data_obj *data)
{
    FILE *target;
    int i, data_amount, instructions_amount, address;
    target = fopen(ob_filename, "w");
    data_amount = instructions_amount = 0;
    address = ADDRESS_STARTS_AT;

    for (i = 0; instructions[i].exists; i++)
    {
        instructions[i].address = address + instructions_amount;
        instructions_amount += instructions[i].lines_no;
        if (strlen(instructions[i].jumping_label))
        {
            instructions[i].jumping_label_address = find_jumping_label_address(instructions, instructions[i].jumping_label);
        }
    }

    for (i = 0; data[i].exists; i++)
    {
        data[i].address = find_instruction_label_address(instructions, data[i].label, address + instructions_amount + data_amount);
        data_amount += data[i].lines_no;
    }

    fprintf(target, "%d %d\n", instructions_amount, data_amount);

    for (i = 0; instructions[i].exists; i++)
    {
        translate_instruction(target, &instructions[i], data, &address);
    }

    for (i = 0; data[i].exists; i++)
    {
        translate_data(target, &data[i], &address);
    }

    fclose(target);
}

void dec_to_special_binary_base(FILE *fileptr, int number, int length)
{
    int i;
    for (i = (length - 1); i >= 0; i--)
    {
        int k;
        k = number >> i;
        if (k & 1)
        {
            fputs(ONE, fileptr);
        }
        else
        {
            fputs(ZERO, fileptr);
        }
    }
}

void translate_instruction(FILE *fileptr, Instruction_obj *object, Data_obj *data, int *address)
{
    int is_jumping_label;
    is_jumping_label = strlen(object->jumping_label);

    fprintf(fileptr, "%d  ", (*address)++);

    printf("command: %d", object->command);
    /*jumping param 1*/
    dec_to_special_binary_base(fileptr, is_jumping_label ? object->source.type : 0, 2);
    /*jumping param 2*/
    dec_to_special_binary_base(fileptr, is_jumping_label ? object->destination.type : 0, 2);
    /*command*/
    dec_to_special_binary_base(fileptr, object->command, 4);
    /*source operand*/
    dec_to_special_binary_base(fileptr, (is_jumping_label ? 0 : object->source.type) - 1, 2);
    /*destination operand*/
    dec_to_special_binary_base(fileptr, (is_jumping_label ? jump : object->destination.type) - 1, 2);
    /*A,R,E*/
    dec_to_special_binary_base(fileptr, 0, 2);

    fputs("\n", fileptr);
    if (is_jumping_label)
    {
        fprintf(fileptr, "%d  ", (*address)++);
        /*address of the jumping label*/
        dec_to_special_binary_base(fileptr, object->jumping_label_address, 12);
        /*A,R,E //TODO: to check if really is*/
        dec_to_special_binary_base(fileptr, 1, 2);
        fputs("\n", fileptr);
    }
    if (object->source.type != null)
    {
        fprintf(fileptr, "%d  ", (*address)++);
        if (object->source.type == reg)
        {
            /*address of the source reg*/
            dec_to_special_binary_base(fileptr, object->source.type == reg ? object->source.value : 0, 6);
            /*address of the dest reg*/
            dec_to_special_binary_base(fileptr, object->destination.type == reg ? object->destination.value : 0, 6);
            /*A,R,E //TODO: to check if really is*/
            dec_to_special_binary_base(fileptr, 0, 2);
        }
        else if (object->source.type == immediate)
        {
            /*immediate value*/
            dec_to_special_binary_base(fileptr, object->source.value, 12);
            /*A,R,E*/
            dec_to_special_binary_base(fileptr, 0, 2);
        }
        else if (object->source.type == label)
        {
            /*immediate value*/
            dec_to_special_binary_base(fileptr, object->source.scope == external_scope ? 0 : data[object->source.value].address, 12);
            /*A,R,E*/
            dec_to_special_binary_base(fileptr, object->source.scope, 2);
        }
        fputs("\n", fileptr);
    }
    if (object->destination.type != null && (object->destination.type != reg || object->source.type != reg))
    {
        fprintf(fileptr, "%d  ", (*address)++);
        if (object->destination.type == reg)
        {
            /*address of the source reg*/
            dec_to_special_binary_base(fileptr, 0, 6);
            /*address of the dest reg*/
            dec_to_special_binary_base(fileptr, object->destination.value, 6);
            /*A,R,E //TODO: to check if really is*/
            dec_to_special_binary_base(fileptr, 0, 2);
        }
        else if (object->destination.type == immediate)
        {
            /*immediate value*/
            dec_to_special_binary_base(fileptr, object->destination.value, 12);
            /*A,R,E*/
            dec_to_special_binary_base(fileptr, 0, 2);
        }
        else if (object->destination.type == label)
        {
            /*immediate value*/
            dec_to_special_binary_base(fileptr, object->destination.scope == external_scope ? 0 : data[object->destination.value].address, 12);
            /*A,R,E*/
            dec_to_special_binary_base(fileptr, object->destination.scope , 2);
        }
        fputs("\n", fileptr);
    }
}

void translate_data(FILE *fileptr, Data_obj *data, int *address)
{
    int i;
    for (i = 0; i < data->lines_no; i++)
    {
        if (data->lines_no > 0)
        {
            fprintf(fileptr, "%d  ", (*address)++);

            dec_to_special_binary_base(fileptr, data->value[i], 14);

            fputs("\n", fileptr);
        }
    }
}

int find_jumping_label_address(Instruction_obj *instructions, char *jumping_label)
{
    int i;
    for (i = 0; instructions[i].exists; i++)
    {
        if (strcmp(instructions[i].label, jumping_label) == 0)
        {
            return i;
        }
    }

    return 0; /*an external label*/
}

int find_instruction_label_address(Instruction_obj *instructions, char *label, int default_val)
{
    int i;
    for (i = 0; instructions[i].exists; i++)
    {
        if (strcmp(instructions[i].label, label) == 0)
        {
            return instructions[i].address;
        }
    }

    return default_val;
}
