#include <stdio.h>
#include <string.h>
#include "objects.h"
#include "commands.h"

enum commands_enum
{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop
};

void validate_instruction(Instruction_obj *instruction, int *is_invalid, int line_no)
{
    char invalid_command[COMMAND_LENGTH] = {0};

    switch (instruction->command)
    {
    case mov:
    case add:
    case sub:
    {
        if (instruction->source.type == null || instruction->destination.type < label)
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    case cmp:
    {
        if (instruction->source.type == null || instruction->destination.type == null)
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    case clr:
    case inc:
    case dec:
    case jmp:
    case not:
    case bne:
    case red:
    case jsr:
    {
        if (!instruction->jumping_label && (instruction->source.type != null || instruction->destination.type < label))
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    case prn:
    {
        if (instruction->source.type != null || instruction->destination.type == null)
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    case rts:
    case stop:
    {
        if (instruction->source.type != null || instruction->destination.type != null)
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    case lea:
    {
        if ((instruction->source.type != jump && instruction->source.type != label) || instruction->destination.type < label)
        {
            strcpy(invalid_command, commands[instruction->command]);
        }
        break;
    }
    }
    if (strlen(invalid_command))
    {
        printf("Error: invalid addressing mode. for command %s, on line %d\n", invalid_command, line_no);
        *is_invalid = 1;
    }
}

int find_label(char *label, Instruction_obj *instructions)
{
    int i;
    for (i = 0; instructions[i].exists; i++)
    {
        if (strcmp(instructions[i].label, label) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int find_label_in_data(char *label, Data_obj *data)
{
    int i;
    for (i = 0; data[i].exists; i++)
    {
        if (strcmp(data[i].label, label) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void validate_label_operand(Instruction_obj *instructions, Data_obj *data, int *is_invalid)
{
    int i;
   for (i = 0; instructions[i].exists; i++)
    {
        if (strlen(instructions[i].jumping_label) && !find_label(instructions[i].jumping_label, instructions) && !find_label_in_data(instructions[i].jumping_label, data))
        {
            printf("Error: Unkown jumping label \"%s\".\n", instructions[i].jumping_label);
            *is_invalid = 1;
        }
    }
}
