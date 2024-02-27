#include <stdio.h>
#include <stdlib.h>

// max line for reading file
#define MAX_LEN 128
FILE *get_file(char *fname) {
  // open file in read mode
  FILE *fp = fopen(fname, "r");
  if (fp == NULL) {
    perror("Problem occurred when opening a file");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  // return no a
  return fp;
}
// handle byte read error file ther are any
void handle_byte_err(ssize_t bytes, FILE *file) {
  if (bytes == -1) {
    perror("Problem occured reading a bye");
    fclose(file);
    exit(EXIT_FAILURE);
  }
}

const char *read_line(FILE *file) {
  // character buffer to grab each line in file
  // pointer to open file
  ssize_t bytes;
  char *line_buf;
  size_t len = 0;

  if (file == NULL) {
    perror("File pointer is not initialized");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  int length = MAX_LEN;
  bytes = getline(&line_buf, &len, file);
  handle_byte_err(bytes, file);
  return line_buf;
}
