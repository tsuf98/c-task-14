#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "objects.h"
#include "lang.h"
#include "commands.h"

int is_instruction_line(char *line);
int is_data_line(char *line);
int is_empty_line(char *line);
int is_comment_line(char *line);
Data_obj *find_data_by_label(Data_obj *data, int *length, char *label);
void parse_data_line(char *line, int line_number, int *is_invalid, Data_obj *obj, int *objects_length);
char *look_for_label(char *line);
int *look_for_string(char *line, int *length);
char *look_for_value(char *line);
int *look_for_data(char *line, int *length);
int look_for_scope_or_type(char *line);
void trim(char *line);
void cut(char *line, int steps);

Instruction_obj *parse_instructions(FILE *fileptr, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    char word[WORD_LENGTH];
    Instruction_obj *obj = malloc(sizeof(Instruction_obj) * length);

    printf("parsing instructions...\n");
    /* parse and assign each line to it's own object*/
    while (fgets(line, sizeof(line), fileptr))
    {
        if (!is_comment_line(line) && is_instruction_line(line))
        {
        }
    }
    rewind(fileptr);
    return obj;
}

Data_obj *parse_data(FILE *fileptr, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    int line_number, obj_length, i, j;
    Data_obj *obj;
    obj_length = line_number = 0;
    obj = malloc(sizeof(Data_obj) * length);

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
    /*
    for (i = 0; i < obj_length; i++)
    {
        printf("no: %d label: %s scope: %d type: %d line_no: %d\n", i, obj[i].label, obj[i].scope, obj[i].type, obj[i].lines_no);
        for (j = 0; j < obj[i].lines_no; j++)
        {
            printf("    %d", obj[i].value[j]);
        }
        printf("\n");
    }*/

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

    while (fgets(line, sizeof(line), fileptr))
    {
        index++;
        if (is_comment_line(line) || is_empty_line(line))
        {
            continue;
        }
        else if (is_instruction_line(line))
        {
            counter[0]++;
        }
        else if (is_data_line(line))
        {
            counter[1]++;
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
    *length = i + 1;
    return &data[i];
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
        strcpy(label, look_for_value(line));
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
        if ((temp_obj->scope && scope) || (temp_obj->type && type)) {
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

char *look_for_label(char *line)
{
    int i, is_label, is_word;
    char *label = malloc(sizeof(char) * WORD_LENGTH);
    is_label = is_word = 0;
    trim(line);

    for (i = 0; i < strlen(line); i++)
    {
        if (isspace(line[i]) && is_word)
        {
            if (is_label)
            {
                cut(line, i);
                return label;
            }
            return "";
        }
        if (line[i] == LABEL_SEPARATOR)
        {
            is_label = 1;
        }
        else if (!isspace(line[i]))
        {
            is_word = 1;
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

char *look_for_value(char *line)
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
                cut(line, i);
                return ENT_SCOPE;
            }
            else if (strcmp(word, EXTERN) == 0)
            {
                cut(line, i);
                return EXT_SCOPE;
            }
            else if (strcmp(word, DATA) == 0)
            {
                cut(line, i);
                return NUM_TYPE;
            }
            else if (strcmp(word, STRING) == 0)
            {
                cut(line, i);
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

void cut(char *line, int steps)
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

/*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*/