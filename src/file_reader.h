#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
extern ssize_t bytes;
extern FILE *file;
/*FUNCTION DECLARATIONS*/
FILE *get_file(char *name, char *mode);
void handle_byte_error(ssize_t bytes);
const char *read_line(FILE *file);
#endif
