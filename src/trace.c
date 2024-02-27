#include "file_reader.h"
#include <stdint.h>
#include <stdio.h>

#define LOAD_INST 'l'
#define STORE_INST 's'

enum access_t {
  LOAD = 1,
  STORE = 0,
};

// stores information about each instruction
struct inst_t {
  // indicates whether instruction is load or store
  enum access_t access_type;
  // indicates the address of the instruction
  int32_t address;
  // indicates the number of instructions
  uint32_t num_inst;
};

// create an instruction queue to read from
// this is the final data structure we will parse the trace file into
struct inst_queue {
  struct inst_t *next_inst;
  struct inst_t inst_t;
};

// get instruction from file, uses file_reader functions
struct inst_t instruction_from_file(FILE *file) {
  struct inst_t instruction;
  const char *line = read_line(file);
  char access_type;
  int32_t address;
  uint32_t num_inst;
  sscanf(line, "%c%d%u", &access_type, &address, &num_inst);
  if (access_type == LOAD_INST) {
    instruction.access_type = LOAD;
  } else if (access_type == STORE_INST) {
    instruction.access_type = STORE;
  }
  instruction.address = address;
  instruction.num_inst = num_inst;
  return instruction;
}

void print_instructino(struct inst_t inst) {
  switch (inst.access_type) {
  case LOAD:
    printf("Instruction type: LOAD\n");
    break;
  case STORE:
    printf("Instruction type: STORE\n");
    break;
  }
}
