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
// FUNCTION DECLARATIONS
struct cache_t get_cache_config(char *fname);
void print_cache_config(struct cache_t *cache);
