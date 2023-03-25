#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "objects.h"
#include "lang.h"
#include "parse_helpers.h"
#include "validate.h"

void parse_data_line(char *line, int line_number, int *is_invalid, Data_obj *obj, int *objects_length);
Instruction_obj *parse_instruction_line(char *line, int line_number, int *is_invalid, Data_obj *data);
int count_instruction_lines(Instruction_obj *instruction);

Instruction_obj *parse_instructions(FILE *fileptr, Data_obj *data, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    int line_number, obj_length, i;
    Instruction_obj *obj;
    obj_length = line_number = 0;

    obj = calloc(length, sizeof(Instruction_obj));

    printf("parsing instructions...\n");
    /* parse and assign each line to it's own object*/
    while (fgets(line, sizeof(line), fileptr))
    {
        line_number++;
        if (is_comment_line(line) || is_empty_line(line))
        {
            continue;
        }
        if (!is_data_line(line) && is_instruction_line(line)) /*TODO: fix the is_instruction line to work with string tokens*/
        {
            obj[obj_length++] = *parse_instruction_line(line, line_number, is_invalid, data);
        }
    }

    /* print all just once to see how it looks like*/

    for (i = 0; i < obj_length; i++)
    {
        printf("no: %d label: %s (%d) command: %d source: %d-%d jumping label: %s dest: %d-%d \n", obj[i].lines_no, obj[i].label, strlen(obj[i].label), obj[i].command, obj[i].source.type, obj[i].source.value, obj[i].jumping_label, obj[i].destination.type, obj[i].destination.value);
    }

    validate_label_operand(obj, data, is_invalid);

    rewind(fileptr);
    return obj;
}

Data_obj *parse_data(FILE *fileptr, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    int line_number, obj_length, i, j;
    Data_obj *obj;
    obj_length = line_number = 0;
    obj = calloc(length, sizeof(Data_obj));


    printf("parsing data...\n");

    while (fgets(line, sizeof(line), fileptr))
    {
        line_number++;
        if (!is_comment_line(line) && is_data_line(line))
        {
            parse_data_line(line, line_number, is_invalid, obj, &obj_length);
        }
    }
    rewind(fileptr);

    /* print all just once to see how it looks like*/

    for (i = 0; i < obj_length; i++)
    {
        printf("no: %d label: %s scope: %d type: %d line_no: %d\n", i, obj[i].label, obj[i].scope, obj[i].type, obj[i].lines_no);
        for (j = 0; j < obj[i].lines_no; j++)
        {
            printf("    %d", obj[i].value[j]);
        }
        printf("\n");
    }

    return obj;
}

int *count_lines(FILE *fileptr, int *is_invalid)
{
    int index;
    int *counter;
    char line[LINE_LENGTH];
    index = 0;
    counter = malloc(sizeof(int) * 2);
    counter[0] = 0;
    counter[1] = 0;
    printf("counting lines...\n");

    while (fgets(line, sizeof(line), fileptr))
    {
        index++;
        if (is_comment_line(line) || is_empty_line(line))
        {
            continue;
        }
        else if (is_data_line(line))
        {
            counter[1]++;
        }
        else if (is_instruction_line(line))
        {
            counter[0]++;
        }
        else
        {
            printf("Error: invalid line. line %d is not an instruction, empty, comment or data line\n", index);
            *is_invalid = 1;
        }
    }
    rewind(fileptr);
    return counter;
}

void parse_data_line(char *line, int line_number, int *is_invalid, Data_obj *obj, int *objects_length)
{
    char label[WORD_LENGTH];
    int scope, type, value_length, temp_type, is_line_invalid;
    Data_obj *temp_obj;
    int *value = malloc(sizeof(int) * WORD_LENGTH);
    is_line_invalid = scope = type = value_length = temp_type = 0;

    strcpy(label, look_for_label(line));

    if (strlen(label) && !isalpha(label[0]))
    {
        printf("Error: invalid label - must start with an alphanumeric character. on line: %d\n", line_number);
        *is_invalid = 1;
        return;
    }
    temp_type = look_for_scope_or_type(line);

    if (temp_type == EXT_SCOPE || temp_type == ENT_SCOPE)
    {
        scope = temp_type;

        if (strlen(label))
        {
            printf("Warning: meaningless label \"%s\". on line: %d\n", label, line_number);
        }
        strcpy(label, look_for_continues_string(line));
        if (!strlen(label))
        {
            printf("Error: invalid .entry or .extern label. on line: %d\n", line_number);
            is_line_invalid = 1;
        }
    }
    else if (temp_type == STR_TYPE)
    {
        type = temp_type;
        value = look_for_string(line, &value_length);
        if (!value_length)
        {
            printf("Error: invalid .string value. on line: %d\n", line_number);
            is_line_invalid = 1;
        }
    }
    else if (temp_type == NUM_TYPE)
    {
        type = temp_type;
        value = look_for_data(line, &value_length);
        if (!value_length)
        {
            printf("Error: invalid .data value. on line: %d\n", line_number);
            is_line_invalid = 1;
        }
    }
    else
    {
        printf("Error: invalid data line. on line: %d\n", line_number);
        is_line_invalid = 1;
    }

    if (is_line_invalid)
    {
        *is_invalid = 1;
    }
    else
    {
        temp_obj = find_data_by_label(obj, objects_length, label);
        if ((temp_obj->scope && scope) || (temp_obj->value && value))
        {
            printf("Error: duplicated label \"%s\". On line: %d\n", temp_obj->label, line_number);
            *is_invalid = 1;
        }
        else if (scope)
        {
            temp_obj->scope = scope;
        }
        else
        {
            temp_obj->value = malloc(sizeof(int) * value_length);
            temp_obj->value = value;
            temp_obj->lines_no = value_length;
            temp_obj->type = type;
        }
    }
}

Instruction_obj *parse_instruction_line(char *line, int line_number, int *is_invalid, Data_obj *data)
{
    Instruction_obj *instruction;
    instruction = calloc(1, sizeof(Instruction_obj));
    instruction->exists = 1;

    strcpy(instruction->label, look_for_label(line));

    instruction->command = look_for_command(line);
    if (instruction->command == -1)
    {
        printf("Error: unknown command on line: %d\n", line_number);
        *is_invalid = 1;
        return instruction;
    }

    if (strlen(line))
    {
        look_for_params(line, instruction->jumping_label, &instruction->source, &instruction->destination, is_invalid, line_number, data);
    }

    if (strlen(line))
    {
        printf("Error: invalid operand on line: %d\n", line_number);
        *is_invalid = 1;
        return instruction;
    }

    validate_instruction(instruction, is_invalid, line_number);
    instruction->lines_no = count_instruction_lines(instruction);

    return instruction;
}

int count_instruction_lines(Instruction_obj *instruction)
{
    int lines;
    lines = 1;

    if (strlen(instruction->jumping_label))
    {
        lines++;
    }
    if (instruction->source.type == null && instruction->destination.type == null)
    {
        return lines;
    }
    if ((instruction->source.type == null) ^ (instruction->destination.type == null))
    {
        return ++lines;
    }
    if (instruction->source.type == reg && instruction->destination.type == reg)
    {
        return ++lines;
    }
    else
    {
        return lines + 2;
    }
}
/*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*/