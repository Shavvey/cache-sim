#include "file_reader.h"
#include <stdbool.h>
#include <stdint.h>
// address size used for instructions in trace is 32 bits
#define ADDR_SIZE 32
/* cache_t captures the all the important information of the cache setup
 * INCLUDES THE FOLLOWING:
 * LINE SIZE: Specifies the line (block) size for the cache in bytes. Should be
 * a non-negative power of 2
 * ASSOCIATIVITY: Specifies the associativity of the cache, a '1' implies a
 * direct mapped cache, while a '0' implies a fully-associated one
 * DATA SIZE: Specifies the total size of the data in the cache, should be
 * specified in KB. For example, a value of '64' means a 64KB cache
 * REPLACEMENT POLICY: Specifies the replacement policy for the cache.
 * Currently, there's is only FIFO, specified by a '1' and Random, specified by
 * a '0'
 * MISS PENALTY: Specifies the number of cycles penalized on a cache miss. May
 * be any postive integer.
 * WRITE ALLOCATE: Specifies the cache policy on write misses. A value of '0'
 * indicates no-write-allocate. A value of '1' indicates write-allocate
 * Assumes write through is used with no-write-allocate and write-back is used
 * with write-allocate.*/
// represent replacement policy
enum REP_POLICY { RANDOM = 0, FIFO = 1 };
// represent whether we have no-write-allocate or write-allocate cache
enum WRITE_ALLOC { NO = 0, YES = 1 };

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

// get cache config from file, use the file_reader.c libraries created
struct cache_t get_cache_config(char *fname) {
  struct cache_t cache;
  // string that stores each line of config file
  const char *line;
  // get the pointer to the config file, passed through as a param
  FILE *file = get_file(fname);
  // read block size
  line = read_line(file);
  sscanf(line, "%i", &cache.block_size);
  // read associativity (n-way set associativity)
  line = read_line(file);
  sscanf(line, "%i", &cache.associativity);
  // read the total cache size
  line = read_line(file);
  sscanf(line, "%i", &cache.cache_size);
  // read replacement policy
  line = read_line(file);
  sscanf(line, "%d", &cache.rep_policy);
  // read miss penalty
  line = read_line(file);
  sscanf(line, "%i", &cache.miss_penalty);
  // read write allocation policy (no-write-allocate vs write-allocate)
  line = read_line(file);
  sscanf(line, "%d", &cache.write_alloc);
  // return the newly created cache, parsed from file
  return cache;
}

// prints back the cache config, just for testing purposes
void print_cache_config(struct cache_t *cache) {
  printf("Block size: %i Bytes\n", cache->block_size);
  printf("N-way associativity; %i\n", cache->associativity);
  printf("Total cache size: %i KB\n", cache->cache_size);
  switch (cache->rep_policy) {
  case RANDOM:
    printf("Eviction policty: Random\n");
    break;
  case FIFO:
    printf("Eviction policy: FIFO\n");
    break;
  default:
    printf("Couldn't match eviction policy\n");
    break;
  }
  printf("Cycle penalty for cache miss: %i\n", cache->miss_penalty);
  switch (cache->write_alloc) {
  case NO:
    printf("Writes allocated inside cache: NO (no-write-allocate)\n");
    break;
  case YES:
    printf("Writes allocated inside cache: YES (write-allocate)\n");
    break;
  default:
    printf("Couldn't match write allocate policy!\n");
    break;
  }
}
// recursive function to check if a val is a power of two,
// integer have to be type casted to floats before using
// this is used to tell us whether the config is correct
bool check_2pow(float val) {
  if (val == 1) {
    return true;
  } else if (val < 1) {
    return false;
  } else {
    return check_2pow(val / 2);
  }
}

// after we know config is correct,
// what power of two gets the val passed as the arg
// iteration arg should be set to zero when first invoked
int find_pow2(int val, int iter) {
  if (val == 1) {
    return iter;
  } else {
    // increment iteration count
    iter++;
    return find_pow2(val / 2, iter);
  }
}

// determine the cache address dimnesions based on cache config
struct cache_addr_d calculate_cache_dimensions(struct cache_t *cache) {
  struct cache_addr_d addr_d;
  addr_d.offset = find_pow2(cache->block_size, 0);
  addr_d.set_index = find_pow2(cache->associativity, 0);
  addr_d.tag = ADDR_SIZE - addr_d.offset - addr_d.set_index;
  return addr_d;
}

void print_cache_dimensions(struct cache_addr_d addr_d) {
  printf("Number of bits for Tag: %u\n", addr_d.tag);
  printf("Number of bits for Set Index: %u\n", addr_d.set_index);
  printf("Numeber of bits for Offset: %u\n", addr_d.offset);
}
