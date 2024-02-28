#ifndef TRACE_H
#define TRACE_H
#include <stdint.h>
#include <stdio.h>
// TYPE DECLARATIONS
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
// FUNCTION DECLARATIONS
struct inst_t instruction_from_file(FILE *file);
void print_instruction(struct inst_t inst);
void print_all_inst(FILE *file);
uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset);
int power(int base, int exponent);
#endif
