/* language parsing components*/
#include "lang.h"

enum param_type
{
    null,
    immediate,
    label,
    jump,
    reg
};

typedef struct Param
{
    int value;
    char instruction_label[WORD_LENGTH];
    enum param_type type;
} Param;

typedef struct Instruction_obj
{
    int lines_no;
    char label[WORD_LENGTH]; 
    int command;
    char jumping_label[WORD_LENGTH];
    Param source;
    Param destination;
    unsigned int exists:1;

} Instruction_obj;

#define STR_TYPE 1
#define NUM_TYPE 2

#define ENT_SCOPE 10
#define EXT_SCOPE 20

typedef struct Data_obj
{
    int lines_no;
    char label[WORD_LENGTH]; 
    int * value;
    int scope;
    int type; 
    unsigned int exists:1;
} Data_obj;

