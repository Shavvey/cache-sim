#ifndef SIM_H
#define SIM_H
// struct to represent each block in the set
#include "cache.h"
#include <stdbool.h>
#include <stdint.h>
// struct to represent each block in the set
struct blocks_t {
  // holds the 32 bit address
  int32_t addr;
  // also contains a reference to the next block
  struct blocks_t *next;
};

// each set holds a given amount of cache blocks, given by associativity value
struct set_t {
  // each set is represented as a chain of cache blocks
  struct blocks_t *blocks;
  // how many cache blocks are currently in the set
  uint8_t length;
};

// cache is just an array of sets
struct cache {
  struct set_t *set;
};

// FUNCTION DECLARATOINS
void load_cache_config(char *config_fname);
bool compare_tag(struct cache_addr_d addr, uint32_t address1,
                 uint32_t address2);
uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset);
void cache_sim(char *fname);
void record_hit(enum access_type type);
int power_rec(int base, int exponent);
uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset);
void record_miss(enum access_type type);
bool compare_tag(struct cache_addr_d addr, uint32_t address1,
                 uint32_t address2);
void handle_eviction(struct set_t *set, enum rep_policy policy, uint32_t addr);
struct blocks_t *create_block(uint32_t addr);

#endif
