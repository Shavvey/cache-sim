#include "cache.h"
#include "trace.h"
// GLOBALS
// cache configuration
struct cache_t cache_conf;
// cache address dimensions
struct cache_addr_d addr_d;
uint32_t num_sets;

enum block_s { FREE = 0, OCCUPIED = 1 };

struct block_t {
  enum block_s status;
};
// load cache config and address dimensions
void load_cache_config(char *config_fname) {
  cache_conf = get_cache_config(config_fname);
  print_cache_config(&cache_conf);
  addr_d = calculate_cache_dimensions(&cache_conf);
  print_cache_dimensions(addr_d);
}

void get_num_sets() {
  // get the number of set using the block number
  num_sets = (cache_conf.cache_size * 1000) /
             (cache_conf.block_size * cache_conf.associativity);
}
