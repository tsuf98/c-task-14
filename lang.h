/* restricted words and language componenets*/

#define LINE_LENGTH 81
#define WORD_LENGTH 40
#define COMMAND_LENGTH 6 /*max length of a single command string*/

#define COMMANDS_AMOUNT 16 /*amount of commands in the language*/
#define REGISTERS_AMOUNT 8

#define MACRO_START "mcr"
#define MACRO_END "endmcr"

#define MOV "mov"
#define CMP "cmp"
#define ADD "add"
#define SUB "sub"
#define NOT "not"
#define CLR "clr"
#define LEA "lea"
#define INC "inc"
#define DEC "dec"
#define JMP "jmp"
#define BNE "bne"
#define RED "red"
#define PRN "prn"
#define JSR "jsr"
#define RTS "rts"
#define STOP "stop"


#define R0 "r0"
#define R1 "r1"
#define R2 "r2"
#define R3 "r3"
#define R4 "r4"
#define R5 "r5"
#define R6 "r6"
#define R7 "r7"


#define DATA ".data"
#define ENTRY ".entry"
#define STRING ".string"
#define EXTERN ".extern"

#define LABEL_SEPARATOR ':'
#define ARRAY_SEPARATOR ","
#define IMMEDIATE_SEPARATOR '#'
#define OPERAND_SEPARATOR ',' 
#define COMMENT ';'

#define JUMP_START_BRACKET '('
#define JUMP_END_BRACKET ')'
