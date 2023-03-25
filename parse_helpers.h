
/* Parsers */

/* finds a label in a line of string and return it's address*/
/* @param line - an address of a string line */
/* @returns a pointer to the label or to an empty string if none */
char *look_for_label(char *line);

/* finds a string (wrapped in quotation marks) in a line and return it's address*/
/* @param line an address of a string line */
/* @param length will be set to the length of the returned array */
/* @returns an array of ascii values that represents the found string. ends with a '\0'.*/
int *look_for_string(char *line, int *length);

/* @param line an address of a string line */
/* @returns the line if the string is continues, and NULL if not*/
char *look_for_continues_string(char *line);

/* finds a list of numerical values and converts them to array of integers*/
/* @param line an address of a string line */
/* @param length will be set to the length of the returned array */
int *look_for_data(char *line, int *length);

/* finds operands in a line of string and parses them */
/* @param line an address of a string line */
/* @param jumping_label the label to "jump" into */
/* @param source a source param */
/* @param dest a destination param */
/* @param is_invalid validity flag, will set to 1 if the line is invalid */
/* @param line_no a line number */
/* @param data a list of data objects, to extract the label indices from */
void look_for_params(char *line, char *jumping_label, Param *source, Param *dest, int *is_invalid, int line_no, Data_obj *data);

/* finds a command in a string line*/
/* @param line an address of a string line */
/* @returns an int represents the command */
int look_for_command(char *line);

/* finds a the scope or type (.data, .string, .extern, .entry) in  a string line*/
/* @param line an address of a string line */
/* @returns an int represents the type or scope of the line */
int look_for_scope_or_type(char *line);

/*parses a param into a Param structure*/
/*@param word the string to parse*/
/*@param param an address of the param for the information to be saved to*/
/*@param labels a list of data objects*/
/*@param line_number the line number in the file in which the word appears*/
/* @param is_invalid validity flag, will set to 1 if the line is invalid */
/* @param jumping_label to assign if exists */
void parse_param(char *word, Param *param, Data_obj *labels, int line_number, int *is_invalid, char *jumping_label);

/* Finding in arrays */

Data_obj *find_data_by_label(Data_obj *data, int *length, char *label);

int index_of_command(char *word);

int index_of_data_label(char *word, Data_obj *labels);

int index_of_registers(char *word);

/* String trimming */

void cut_beginning(char *line, int steps);

void trim(char *line);

/* Line classification */

int is_instruction_line(char *line);

int is_data_line(char *line);

int is_empty_line(char *line);

int is_comment_line(char *line);