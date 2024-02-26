#include <stdio.h>
#include <stdlib.h>

// max line for reading file
#define MAX_LEN 128
// character buffer to grab each line in file
// pointer to open file
ssize_t bytes;
char *line_buf;
size_t len = 0;

FILE *getFile(char *fname){
  // open file in read mode
  FILE *fp = fopen(fname,"r");  
  if(fp == NULL){
    perror("Error: problem opening file");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  return fp;
}

void handle_byte_err(ssize_t bytes, FILE *file){
  if(bytes == -1){
    perror("Error: problem occured when reading textfile");
    fclose(file);
    exit(EXIT_FAILURE);
  }
}

const char *read_line(FILE *file){
  if(file == NULL){
    perror("File pointer is not initialized!");
    exit(EXIT_FAILURE);
  }
  int length = MAX_LEN;
  bytes = getline(&line_buf,&len,file);
  handle_byte_err(bytes,file);
  return line_buf;
}


