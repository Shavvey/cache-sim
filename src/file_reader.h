/*EXTERN VARS*/
#include <stdio.h>
extern ssize_t bytes;
extern char *line_buf;
extern size_t len;
/*FUNCTION DECLARATIONS*/
FILE* get_file(char *name);
void handle_byte_error(ssize_t bytes);
const char *read_line(FILE *file);

