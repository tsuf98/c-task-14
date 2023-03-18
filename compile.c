#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre_processing.h"
#include "parse.h"
#include "validate.h"
#include "translate.h"
#include "write_files.h"

/**********************************************************************/
/* compile - compiles a assembly file into binaries                   */
/* @param filname - .as file without extention                        */
/*                                                                    */
/**********************************************************************/

void compile(char *filename)
{
    int is_invalid;
    FILE *source;
    char *as_filename, *am_filename, *ob_filename, *ent_filename, *ext_filename;
    int *line_length;
    Instruction_obj *instructions;
    Data_obj *data;

    is_invalid = 0;
    as_filename = malloc(sizeof(char) * (strlen(filename) + 4));

    printf("Reading file: %s\n", filename);
    /*Reading file*/
    strcat(strcpy(as_filename, filename), ".as");
    source = fopen(as_filename, "r");

    if (source == NULL)
    {
        printf("Error: %s doesn't exist.\n", as_filename);
        free(source);
        return;
    }

    /*Pre-processing (inserting macros)*/
    am_filename = malloc(sizeof(char) * (strlen(filename) + 4));
    strcat(strcpy(am_filename, filename), ".am");
    pre_process(source, am_filename);
    fclose(source);

    /*Parse to Line struct*/
    source = fopen(am_filename, "r");

    line_length = count_lines(source, &is_invalid);
    instructions = parse_instructions(source, line_length[0], &is_invalid); /**/
    data = parse_data(source, line_length[1], &is_invalid);
    free(line_length);

    /*Validate*/
    validate_instructions(instructions, &is_invalid); /**/
    validate_data(data, &is_invalid);                 /**/

    if (is_invalid)
    {
        printf("Error: invalid file - %s. output files won't be created - see errors above.\n", as_filename);
        /* free everything */
        free(as_filename); /* TODO: free more stuff **/
        return;
    }

    /*translate to binary*/
    ob_filename = malloc(sizeof(char) * (strlen(filename) + 4));
    strcat(strcpy(as_filename, filename), ".ob");

    translate_to_binary(ob_filename, instructions, data); /**/

    /*Saving internals */
    ent_filename = malloc(sizeof(char) * (strlen(filename) + 5));
    strcat(strcpy(as_filename, filename), ".ent");

    save_internals(ent_filename, instructions, data); /**/

    /*Saving externals*/
    ext_filename = malloc(sizeof(char) * (strlen(filename) + 5));
    strcat(strcpy(as_filename, filename), ".ext");

    save_externals(ext_filename, instructions, data); /**/

    /* free everything */
    free(instructions);
    free(data);
    /*free(am_filename);
    free(as_filename);
    free(ob_filename);
    free(ext_filename);
    free(ent_filename);*/

    printf("Done reading file: %s\n", filename);
}
