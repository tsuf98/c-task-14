#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lang.h"

typedef struct Macro
{
    char name[WORD_LENGTH];
    char *value;
} Macro;

/* (private) estimates the macro sizee and initiate an array of macros */
Macro *estimate_macros_size(FILE *fileptr);

/*  (private) extract the macro name and assigns it to a macro in the list */
void get_macro_name(char line[LINE_LENGTH], char *name);

/*  (private) extract the macro name and assigns it to a macro in the list */
void get_macro_name(char line[LINE_LENGTH], char *name);

/*  (private) passes through the file and collects the macros to the dedicated list*/
int collect_macros(FILE *fileptr, Macro *macros);

/* **************************************************************************************** */
/* pre_process- handles the pre processing part- parsing the macros and creates an .am file */
/* @param FILE * fileptr- a pointer to the source file                                      */
/* @param char * target_filename- name of the .am target file to be created                 */
/* @ returns void                                                                           */
/* **************************************************************************************** */
void pre_process(FILE *fileptr, char *target_filename)
{
    FILE *target;
    int macros_length, i, is_in_macro;
    char line[LINE_LENGTH];
    Macro *macros = estimate_macros_size(fileptr);
    macros_length = collect_macros(fileptr, macros);
    target = fopen(target_filename, "w");
    is_in_macro = 0;

    for (i = 0; i < macros_length; i++)
    {
        printf("Found macro (%d): %s --> \n\"%s\"\n", i + 1, macros[i].name, macros[i].value);
    }

    /*reading line by line*/
    while (fgets(line, sizeof(line), fileptr))
    {
        if (strstr(line, MACRO_START))
        {
            is_in_macro = strstr(line, MACRO_END) ? 0 : 1;
        }
        else if (!is_in_macro)
        {
            int isMacroInLine;
            isMacroInLine = 0;
            for (i = 0; i < macros_length; i++)
            {
                if (strstr(line, macros[i].name))
                {
                    isMacroInLine = 1;
                    fputs(macros[i].value, target);
                }
            }
            if (!isMacroInLine)
            {
                fputs(line, target);
            }
        }
    }
    fclose(target);
    free(macros);
}

/* (private) estimates the macro sizee and initiate an array of macros */
Macro *estimate_macros_size(FILE *fileptr)
{
    Macro *macros;
    int largest_macro, temp, macro_counter, is_in_macro, i;
    char line[LINE_LENGTH];

    largest_macro = temp = macro_counter = is_in_macro = 0;

    while (fgets(line, sizeof(line), fileptr))
    {
        if (strstr(line, MACRO_START) != NULL)
        {
            if (strstr(line, MACRO_END) != NULL)
            {
                if (temp > largest_macro)
                {
                    largest_macro = temp;
                }
                is_in_macro = 0;
            }
            else
            {
                macro_counter++;
                temp = 0;
                is_in_macro = 1;
            }
        }
        else if (is_in_macro)
        {
            temp++;
        }
    }
    rewind(fileptr);
    macros = malloc(sizeof(Macro) * macro_counter);
    for (i = 0; i < macro_counter; i++)
    {
        macros[i].value = malloc(sizeof(char) * LINE_LENGTH * largest_macro);
    }
    return macros;
}

/*  (private) extract the macro name and assigns it to a macro in the list */
void get_macro_name(char line[LINE_LENGTH], char *name)
{
    int is_macro;
    char word[WORD_LENGTH];
    is_macro = 0;
    while (sscanf(line, "%s", word) == 1)
    {

        line = &line[strlen(word) + 1];
        if (strcmp(word, MACRO_START))
        {
            is_macro = 1;
        }
        else if (is_macro)
        {
            break;
        }
    }
    strcpy(name, word);
}

/*  (private) passes through the file and collects the macros to the dedicated list*/
int collect_macros(FILE *fileptr, Macro *macros)
{
    int index, is_in_macro;
    char line[LINE_LENGTH];
    index = is_in_macro = 0;
    while (fgets(line, sizeof(line), fileptr))
    {
        if (strstr(line, MACRO_START) != NULL)
        {
            if (strstr(line, MACRO_END) != NULL)
            {
                index++;
                is_in_macro = 0;
            }
            else
            {
                get_macro_name(line, macros[index].name);
                is_in_macro = 1;
            }
        }
        else if (is_in_macro)
        {
            strcat(macros[index].value, line);
        }
    }
    rewind(fileptr);
    return index;
}
