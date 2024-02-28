#ifndef SIM_H
#define SIM_H
// struct to represent each block in the set
#include <stdbool.h>
#include <stdint.h>
struct blocks_t {
  // holds the 32 bit address
  int32_t addr;
  // also contains a reference to the next block
  struct blocks_t *next;
};
// each set holds a given ammount of cache blocks, given by associativity value
struct set_t {
  // each set is represented as a chain of cache blocks
  struct blocks_t *blocks;
  // how many cache blocks are currently in the set
  uint8_t length;
};
// FUNCTION DECLARATOINS
void load_cache_config(char *config_fname);
void cache_sim();
bool compare_tag(struct cache_addr_d addr, uint32_t address1,
                 uint32_t address2);

uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset);
void cache_sim(char *fname);
#endif
