#ifndef FILE_READER_H
#define FILE_READER_H
#include <stdio.h>
extern ssize_t bytes;
extern FILE *file;
/*FUNCTION DECLARATIONS*/
FILE *get_file(char *name, char *mode);
void handle_byte_error(ssize_t bytes);
const char *read_line(FILE *file);
void handle_byte_err(ssize_t bytes);
void write_line(FILE *file, char *line);
void strip_ext(char *fname);
void create_output_fname(char *fname, char *new_ext);
char *strip_path_info(char *fname);
#endif
