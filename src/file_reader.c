#include <stdio.h>
#include <stdlib.h>

// max line for reading file
#define MAX_LEN 128
ssize_t bytes;
FILE *file;
FILE *get_file(char *fname, char *mode) {
  // open file in read mode
  file = fopen(fname, mode);
  if (file == NULL) {
    perror("Problem occurred when opening a file");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  // return no a
  return file;
}
// handle byte read error file ther are any
void handle_byte_err(ssize_t bytes) {
  if (bytes == -1) {
    // perror("Problem occured reading a byte\n");
  }
}

const char *read_line(FILE *file) {
  // character buffer to grab each line in file
  // pointer to open file
  char *line_buf = NULL;
  size_t len;
  if (file == NULL) {
    perror("File pointer is not initialized\n");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  bytes = getline(&line_buf, &len, file);
  handle_byte_err(bytes);
  return line_buf;
}
