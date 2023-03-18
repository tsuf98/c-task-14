/* language parsing components*/
#include "lang.h"

enum param_type
{
    reg,
    num,
    str
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


typedef struct Data_obj
{
    int lines_no;
    char label[WORD_LENGTH]; 
    Param data;
} Data_obj;

