#include "objects.h"

Instruction_obj *parse_instructions(FILE *fileptr, Data_obj * data, int length, int * is_invalid);

Data_obj *parse_data(FILE *fileptr, int length, int * is_invalid);

int *count_lines(FILE *fileptr, int *is_invalid);
