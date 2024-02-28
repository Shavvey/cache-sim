#ifndef CACHE_H
#define CACHE_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define LOAD_INST 'l'
#define STORE_INST 's'

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

// FUNCTION DECLARATIONS
struct cache_t get_cache_config(char *fname);
void print_cache_config(struct cache_t *cache);
bool check_2pow(float val);
int find_pow2(int val, int iter);
struct cache_addr_d calculate_cache_dimensions(struct cache_t cache);
void print_cache_dimensions(struct cache_addr_d);
struct inst_t instruction_from_file(FILE *file);
void print_instruction(struct inst_t inst);
void print_all_inst(FILE *file);
#endif
