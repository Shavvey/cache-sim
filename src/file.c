#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*************************************************************************
Filename: file.c
Description: Implements file writer and reader functions.
This is used to read the cache config and the trace file.
Author: Cole Johnosn
Class: CSE 3031
Instructor: Zheng
Assignment: Lab #2
Assigned: 2/22/2024
Due: 3/7/2024
************************************************************************/
// max line for reading file
#define MAX_LEN 128
// bytes read from currently opened file
ssize_t bytes;
// file pointer to currently opened file
FILE *file;
FILE *get_file(char *fname, char *mode) {
  // open file in read mode
  file = fopen(fname, mode);
  if (file == NULL) {
    perror("Problem occurred when opening a file");
    // exit out of program
    exit(EXIT_FAILURE);
  }
  // return file pointer
  return file;
}
// handle byte read error file ther are any
void handle_byte_err(ssize_t bytes) {
  if (bytes == -1) {
    // print out an error if we encountered a error when reading bytes from a
    // file
    perror("Problem occured reading a byte\n");
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
  return line_buf;
}

// just a little helper function to write a given string to a textfile
void write_line(FILE *file, char *line) { fprintf(file, "%s", line); }

// strip the filename extension off of a string,
// this just loop through the string until in finds a '.'
// and then just null terminates it afterwards
void strip_ext(char *fname) {
  // find the end of the original string using strlen
  char *end = fname + strlen(fname);
  while (end > fname && *end != '.') {
    // decrement pointer to the end of the string
    --end;
  }
  if (end > fname) {
    // null terminate it if we have found the '.' and the end pointer is not the
    // first char in the string
    *end = '\0';
  }
}

// create the file name that we write the cache sim statistics to, should be
// based on the instruction trace file name (e.g. gcc.trace -> gcc.out)
void create_output_fname(char *fname, char *new_ext) {
  // strip off any file extensions
  strip_ext(fname);
  // concatenate with the new file extension name
  strcat(fname, new_ext);
}

// this is just a simple helper function to create a new string
// that strips off extra path information
// what i would like this function to do ../trace/gcc.trace -> gcc.trace
char *strip_path_info(char *fname) {
  int i = 0;
  int index = 0;
  for (char *ps = fname; *ps != '\0'; ps++) {
    // find the last instance of the '\' char
    if (*ps == '/') {
      index = i;
    }
    i++;
  }
  char *name = fname;
  // skipt over the last '/' char, creating a file name without any path info
  name += index + 1;
  return name;
}
