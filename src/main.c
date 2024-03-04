#include "sim.h"
#include <stdio.h>
#include <stdlib.h>
/*************************************************************************
Filename: main.c
Description: Contains main function that drivers the cache simulator.
Author: Cole Johnosn
Class: CSE 3031
Instructor: Zheng
Assignment: Lab #2
Assigned: 2/22/2024
Due: 3/7/2024
************************************************************************/

// configuration file is the first passed arg
#define CONFIG_FILE 1
// trace file is the second passed arg
#define TRACE_FILE 2

int main(int argc, char *argv[]) {
  // argc will track to see if a arguments have been passed
  // 1: name of textfile
  // 2: name of cache config file
  // 3: name of load-store instruction trace file
  if (argc > 2) {
    char *cname = argv[CONFIG_FILE];
    char *tname = argv[TRACE_FILE];
    load_cache_config(cname);
    cache_sim(tname);

  } else {
    // print this error message if we aren't given the proper amount of args
    const char *ERROR =
        "Need two arguments passed to run the cache simulator:\n\
    1) the name of the config file\n\
    2) the name of the trace file\n";
    // print out the error
    perror(ERROR);
    return EXIT_FAILURE;
  }
  // exit out of program with success code
  return EXIT_SUCCESS;
}
