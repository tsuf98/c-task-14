#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "objects.h"
#include "lang.h"
#include "commands.h"
#include "parse_helpers.h"


char *look_for_label(char *line)
{
    int i, is_label;
    char *label;
    label = calloc(WORD_LENGTH, sizeof(char));
    is_label = 0;
    trim(line);

    for (i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]))
        {
            if (is_label)
            {
                cut_beginning(line, i);
                return label;
            }
            return "";
        }
        if (line[i] == LABEL_SEPARATOR)
        {
            is_label = 1;
        }
        else
        {
            label[i] = line[i];
        }
    }
    return "";
}

int *look_for_string(char *line, int *length)
{
    int *value = malloc(sizeof(int) * WORD_LENGTH);
    int i;

    trim(line);
    if ((line[0] == '\"' && line[strlen(line) - 1] == '\"'))
    {
        line = &line[1];
        line[strlen(line) - 1] = '\0';
        for (i = 0; i <= strlen(line); i++)
        {
            value[i] = line[i]; /*converts to ascii code*/
        }
        *length = i;
        return value;
    }
    return 0;
}

char *look_for_continues_string(char *line)
{
    int i;
    trim(line);
    for (i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]))
        {
            return NULL;
        }
    }
    return line;
}

int *look_for_data(char *line, int *length)
{
    int i, last_comma;

    char *token;
    int *value = malloc(sizeof(int) * WORD_LENGTH);

    trim(line);

    /* detect duplicated commas*/
    for (i = 0; i < strlen(line); i++)
    {
        if (isdigit(line[i]))
        {
            last_comma = 0;
        }
        else if (line[i] == ',')
        {
            if (last_comma)
            {
                return NULL;
            }
            last_comma = 1;
        }
    }

    token = strtok(line, ARRAY_SEPARATOR);

    i = 0;
    /* walk through other tokens */
    while (token != NULL)
    {
        value[i++] = atoi(token);
        token = strtok(NULL, ARRAY_SEPARATOR);
    }
    *length = i;
    return value;
}

int look_for_scope_or_type(char *line)
{
    char word[WORD_LENGTH] = {0};
    int i;

    trim(line);

    for (i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]))
        {
            if (strcmp(word, ENTRY) == 0)
            {
                cut_beginning(line, i);
                return ENT_SCOPE;
            }
            else if (strcmp(word, EXTERN) == 0)
            {
                cut_beginning(line, i);
                return EXT_SCOPE;
            }
            else if (strcmp(word, DATA) == 0)
            {
                cut_beginning(line, i);
                return NUM_TYPE;
            }
            else if (strcmp(word, STRING) == 0)
            {
                cut_beginning(line, i);
                return STR_TYPE;
            }
        }
        else
        {
            word[i] = line[i];
        }
    }

    return 0;
}

int look_for_command(char *line)
{
    char word[WORD_LENGTH] = {0};
    int i, index;

    trim(line);

    for (i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]))
        {
            cut_beginning(line, i);
            return index_of_command(word);
        }
        else
        {
            word[i] = line[i];
        }
    }

    index = index_of_command(word);
    if (index != -1)
    {
        cut_beginning(line, i);
    }
    return index;
}

void look_for_params(char *line, char *jumping_label, Param *source, Param *dest, int *is_invalid, int line_no, Data_obj *data)
{
    int i, j, is_space_valid;
    char word[WORD_LENGTH] = {0};
    is_space_valid = 1;
    j = 0;

    trim(line);

    for (i = 0; i < strlen(line); i++)
    {
        if (line[i] == JUMP_START_BRACKET)
        {
            is_space_valid = 0;
            strcpy(jumping_label, word);
            memset(word, 0, sizeof(word));
            j = 0;
        }
        else if (line[i] == OPERAND_SEPARATOR)
        {
            is_space_valid = 1;
            parse_param(word, source, data, line_no, is_invalid);
            memset(word, 0, sizeof(word));
            j = 0;
        }
        else if (line[i] == JUMP_END_BRACKET)
        {
            is_space_valid = 1;
            parse_param(word, dest, data, line_no, is_invalid);
            memset(word, 0, sizeof(word));
            j = 0;
        }
        else if (!isspace(line[i]))
        {
            word[j] = line[i];
            j++;
        }
        else if (!is_space_valid)
        {
            printf("Error: misplaced space between operands. on line %d\n", line_no);
            *is_invalid = 1;
        }
    }
    if (strlen(word))
    {
        parse_param(word, dest, data, line_no, is_invalid);
    }
    cut_beginning(line, i);
}

Data_obj *find_data_by_label(Data_obj *data, int *length, char *label)
{
    int i;
    for (i = 0; i < *length; i++)
    {
        if (strcmp(data[i].label, label) == 0)
        {
            return &data[i];
        }
    }
    strcpy(data[i].label, label);
    data[i].type = 0;
    data[i].lines_no = 0;
    data[i].scope = 0;
    data[i].value = 0;
    data[i].exists = 1;
    *length = i + 1;
    return &data[i];
}

void trim(char *line)
{
    {
        char *ptr = malloc(strlen(line) * sizeof(char));
        int length;
        ptr = line;
        length = strlen(ptr);

        while (isspace(ptr[length - 1]))
            ptr[--length] = 0;
        while (*ptr && isspace(*ptr))
            ++ptr, --length;

        memmove(line, ptr, length + 1);
    }
}

void cut_beginning(char *line, int steps)
{
    {
        char *ptr = malloc(strlen(line) * sizeof(char));
        int length, i;
        ptr = line;
        length = strlen(ptr);

        i = 0;
        while (*ptr && i < steps)
            ++ptr, --length, i++;

        memmove(line, ptr, length + 1);
    }
}

int index_of_registers(char *word)
{
    char registers[REGISTERS_AMOUNT][3] = {R0, R1, R2, R3, R4, R5, R6, R7};

    int i;
    for (i = 0; i < REGISTERS_AMOUNT; i++)
    {
        if (strcmp(word, registers[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

int index_of_command(char *word)
{
    int i;
    for (i = 0; i < COMMANDS_AMOUNT; i++)
    {

        if (strcmp(commands[i], word) == 0)
        {

            return i;
        }
    }
    return -1;
}

int index_of_data_label(char *word, Data_obj *labels)
{
    int i;
    for (i = 0; labels[i].exists == 1; i++)
    {
        if (strcmp(word, labels[i].label) == 0)
        {
            return i;
        }
    }
    return -1;
}

int is_instruction_line(char *line)
{
    int i;
    for (i = 0; i <= COMMANDS_AMOUNT; i++)
    {

        if (strstr(line, commands[i]))
        {
            return 1;
        }
    }
    return 0;
}

int is_data_line(char *line)
{
    return strstr(line, ENTRY) || strstr(line, EXTERN) || strstr(line, DATA) || strstr(line, STRING) ? 1 : 0;
}

int is_comment_line(char *line)
{
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (!isspace(line[i]))
        {
            if (line[i] == COMMENT)
            {
                return 1;
            }
        }
    }
    return 0;
}

int is_empty_line(char *line)
{
    int i;
    for (i = 0; i < strlen(line); i++)
    {
        if (!isspace(line[i]))
        {
            return 0;
        }
    }
    return 1;
}


void parse_param(char *word, Param *param, Data_obj *labels, int line_number, int *is_invalid)
{
    int index;
    if (word[0] == IMMEDIATE_SEPARATOR)
    {
        cut_beginning(word, 1);
        param->value = atoi(word);
        param->type = immediate;
    }
    else
    {
        index = index_of_registers(word);
        if (index != -1)
        {
            param->value = index;
            param->type = reg;
        }
        else
        {
            index = index_of_data_label(word, labels);
             param->type = label;
            if (index != -1)
            {
                param->value = index;
            }
            else
            {
                strcpy(param->instruction_label, word);
            }
        }
    }
}
