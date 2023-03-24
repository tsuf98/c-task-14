#include <stdio.h>
#include <string.h>
#include "objects.h"

void validate_instructions(Instruction_obj *instructions, int* is_invalid, int line_no)
{
    *is_invalid = 0;
}

int find_label(char * label, Instruction_obj * instructions, int length) {
int i;
    for (i = 0; i < length; i++) {
        if (strcmp(instructions[i].label, label) == 0) {
            return 1;
        }
    }
    return 0;
}

void validate_label_operand(Instruction_obj *instructions,int length, int* is_invalid)
{
    int i;
    for (i = 0; i < length; i++) {
        if (strlen(instructions[i].jumping_label) && !find_label(instructions[i].jumping_label, instructions, length)) {
            printf("Error: Unkown jumping label \"%s\".\n", instructions[i].jumping_label);
            *is_invalid = 1;
        }

         if (strlen(instructions[i].source.instruction_label) && !find_label(instructions[i].source.instruction_label, instructions, length)) {
            printf("Error: Unkown source label operand \"%s\".\n", instructions[i].source.instruction_label);
            *is_invalid = 1;
        }

         if (strlen(instructions[i].destination.instruction_label) && !find_label(instructions[i].destination.instruction_label, instructions, length)) {
            printf("Error: Unkown destination label operand \"%s\".\n", instructions[i].destination.instruction_label);
            *is_invalid = 1;
        }
    }
}
