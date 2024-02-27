#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
/*FUNCTION DECLARATIONS*/
FILE *get_file(char *name);
void handle_byte_error(ssize_t bytes);
const char *read_line(FILE *file);
#endif
