#include "cache.h"
#include "file_reader.h"
#include "trace.h"
#include <stdlib.h>
#define BLOCK_SIZE sizeof(struct block_t *)
#define SET_SIZE sizeof(struct set_t *)
// GLOBALS
// cache configuration
struct cache_t cache_conf;
// cache address dimensions
struct cache_addr_d addr_d;
uint32_t num_sets;
uint32_t num_blocks;

// struct to represent each block in the set
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

// cache is just an array of sets
struct cache {
  struct set_t *set;
};

// load cache config and address dimensions
void load_cache_config(char *config_fname) {
  cache_conf = get_cache_config(config_fname);
  print_cache_config(&cache_conf);
  addr_d = calculate_cache_dimensions(cache_conf);
  print_cache_dimensions(addr_d);
  // get the number of set using the block number
  num_sets = (cache_conf.cache_size * 1024) /
             (cache_conf.block_size * cache_conf.associativity);
  num_blocks = (cache_conf.cache_size * 1024) / (cache_conf.block_size);
  printf("Number of sets: %d\n", num_sets);
  printf("Number of blocks: %d\n", num_blocks);
}
// after loading in cache config we can finally perform the cache simulation
void cache_sim(char *trace_fname) {
  // declare array of sets to represent cache, based on number of set obtained
  // from cache config
  // create cache struct that represents the state of the cache
  struct cache cache[num_sets];
  // allocate memory for each set, leave the block pointer set to null for now
  for (int i = 0; i < num_sets; i++) {
    cache[i].set = (struct set_t *)malloc(SET_SIZE);
    if (cache[i].set != NULL) {
      // set the length of the set to be zero explicitly
      cache[i].set->length = 0;
      // set the pointer to the chain of blocks to NULL explicitly
      cache[i].set->blocks = NULL;
    }
  }
  FILE *trace_file = get_file(trace_fname);
  while (!feof(trace_file)) {
    // get the instruction from the trace file
    struct inst_t inst = instruction_from_file(trace_file);
    // **PROCESSING THE INSTRUCTION**
  }
}
