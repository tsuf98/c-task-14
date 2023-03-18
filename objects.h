/* language parsing components*/
#include "lang.h"

enum param_type
{
    reg,
    num,
    str,
    label
};

enum param_scope
{
    internal,
    external
};

typedef struct Param
{
    char value[WORD_LENGTH];
    enum param_type type;
    enum param_scope scope;
} Param;

typedef struct Instruction_obj
{
    int lines_no;
    char label[WORD_LENGTH]; 
    char command[COMMAND_LENGTH]; 
    Param target;
    Param destination;

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
} Data_obj;

