#ifndef CACHE_H
#define CACHE_H
#include <stdbool.h>
#include <stdint.h>
// TYPE DECLARATIONS
// represent replacement policy
enum REP_POLICY { RANDOM = 0, FIFO = 1 };
// represent whether we have no-write-allocate or write-allocate cache
enum WRITE_ALLOC { NO = 0, YES = 1 };
// represents cache configuration, read from text file
struct cache_t {
  uint32_t block_size;
  uint32_t associativity;
  uint32_t cache_size;
  enum REP_POLICY rep_policy;
  uint32_t miss_penalty;
  enum WRITE_ALLOC write_alloc;
};

// used to group together the cache address dimensions
struct cache_addr_d {
  // used to compare different cache blocks
  uint32_t tag;
  // used to indicate what associative set the cache block belongs to
  uint32_t set_index;
  // indicates the size of the cache, part of the lower bits
  uint32_t offset;
};

// FUNCTION DECLARATIONS
struct cache_t get_cache_config(char *fname);
void print_cache_config(struct cache_t *cache);
bool check_2pow(float val);
int find_pow2(int val, int iter);
struct cache_addr_d calculate_cache_dimensions(struct cache_t *cache);
void print_cache_dimensions(struct cache_addr_d);
#endif
