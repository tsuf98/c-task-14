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
is_label(char * word, int index);
Data_obj * find_data_by_label(Data_obj * data, int length, char * label);

Instruction_obj *parse_instructions(FILE *fileptr, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    char word[WORD_LENGTH];
    int index;
    Instruction_obj *obj = malloc(sizeof(Instruction_obj) * length);
    printf("parsing instructions...\n");
    /* parse and assign each line to it's own object*/
    while (fgets(line, sizeof(line), fileptr))
    {
        if (!is_comment_line(line) && is_instruction_line(line))
        {
            while (sscanf(line, "%s", word) == 1)
            {
                //
            }
        }
    }
    rewind(fileptr);
    return obj;
}

Data_obj *parse_data(FILE *fileptr, int length, int *is_invalid)
{
    char line[LINE_LENGTH];
    char word[WORD_LENGTH];
    int word_index;
    int arr_index;
    Data_obj *obj = malloc(sizeof(Data_obj) * length);
    Data_obj *temp = malloc(sizeof(Data_obj));

    printf("parsing data...\n");

    while (fgets(line, sizeof(line), fileptr))
    {
        if (!is_comment_line(line) && is_data_line(line))
        {
            word_index = 0;
            while (sscanf(line, "%s", word) == 1)
            {
                if (is_label(word, word_index)) {
                    temp = find_data_by_label(obj)
                }
                word_index++;
            }
        }
    }
    rewind(fileptr);
    /* parse and assign each line to it's own object*/
    free(temp);
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
            if (line[i] == ';')
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

int is_label(char * word, int index) {
    return index == 0 && strstr(word, ":");
}

Data_obj * find_data_by_label(Data_obj * data, int length, char * label) {
    int i;
    for (i = 0; i < length; i++) {
        if (strcmp(data[i].label, label)) {
            return &data[i];
        }
    }
    data[i].label = label;
    return data[i];
}

