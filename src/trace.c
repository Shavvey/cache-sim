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
  struct inst_t front, rear;
  struct ins_t *array;
  struct inst_t inst_t;
  unsigned capacity;
  unsigned size;
};

// get instruction from file, uses file_reader functions
struct inst_t instruction_from_file(FILE *file) {
  struct inst_t instruction;
  const char *line = read_line(file);
  char access_type;
  int32_t address;
  uint32_t num_inst;
  sscanf(line, "%c %x %u", &access_type, &address, &num_inst);
  if (access_type == LOAD_INST) {
    instruction.access_type = LOAD;
  } else if (access_type == STORE_INST) {
    instruction.access_type = STORE;
  }
  instruction.address = address;
  instruction.num_inst = num_inst;
  return instruction;
}

// print out the read instruction, for testing purposes
void print_instruction(struct inst_t inst) {
  switch (inst.access_type) {
  case LOAD:
    printf("Instruction type: LOAD\n");
    break;
  case STORE:
    printf("Instruction type: STORE\n");
    break;
  default:
    printf("Couldn't match instruction type\n");
    break;
  }
  printf("Address: %x\n", inst.address);
  printf("Instructions since last memory access: %u\n", inst.num_inst);
}

void print_all_inst(FILE *file) {
  while (!feof(file)) {
    struct inst_t inst = instruction_from_file(file);
    print_instruction(inst);
  }
}
int power(int base, int exponent) {
  return (exponent == 1) ? base : base * power(base, exponent - 1);
}
// make a bit mask based on the number of bits needed and the offset
uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset) {
  uint32_t mask = power(2, num_bits) - 1;
  printf("mask: %d\n", mask);
  return mask << offset;
}
