#ifndef SIM_H
#define SIM_H
enum block_s { FREE = 0, OCCUPIED = 1 };

struct block_t {
  enum block_s status;
};
// FUNCTION DECLARATOINS
void load_cache_config(char *config_fname);

#endif
