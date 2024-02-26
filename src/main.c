#include <stdlib.h>
#include "file_reader.h"
#define CONFIG_FILE 1
#define TRACE_FILE 2

int main(int argc, char *argv[]) {
  // argc will track to see if a arguments have been passed 
  // 1: name of textfile
  // 2: name of cache config file
  // 3: name of load-store instruction trace file?
  if(argc > 2 ){
    char *config_fname = argv[CONFIG_FILE];
    char *trace_fname = argv[TRACE_FILE];
    printf("Config file: %s\n",config_fname);
    printf("Trace file: %s\n",trace_fname);
    FILE* cfile = get_file(config_fname);
    FILE* tfile = get_file(trace_fname);
    const char *line = read_line(cfile);
    printf("line: %s\n",line);

  }else{
    // print this error message if we aren't given the proper amount of args
    const char *ERROR = 
    "Need two arguments passed to run the cache simulator:\n\
    1) the name of the config file\n\
    2) the name of the trace file\n";
    // print out the
    perror(ERROR);
  }
  
  // exit out of program with success code
  return EXIT_SUCCESS;
}
