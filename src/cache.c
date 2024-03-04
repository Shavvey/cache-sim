#include "cache.h"
#include "file.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
/*************************************************************************
Filename: cache.c
Description: Implements a struct that stores the cache configuration.
Author: Cole Johnosn
Class: CSE 3031
Instructor: Zheng
Assignment: Lab #2
Assigned: 2/22/2024
Due: 3/7/2024
************************************************************************/
// get cache config from file, use the file_reader.c libraries created
struct cache_t get_cache_config(char *fname) {
  struct cache_t cache;
  // string that stores each line of config file
  const char *line;
  // get the pointer to the config file, passed through as a param
  file = get_file(fname, "r");
  // read block size
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%i", &cache.block_size);
  // read associativity (n-way set associativity)
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%i", &cache.associativity);
  // read the total cache size
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%i", &cache.cache_size);
  // read replacement policy
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%d", &cache.rep_policy);
  // read miss penalty
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%i", &cache.miss_penalty);
  // read write allocation policy (no-write-allocate vs write-allocate)
  line = read_line(file);
  handle_byte_err(bytes);
  sscanf(line, "%d", &cache.write_alloc);
  // close file being read
  fclose(file);
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
struct cache_addr_d calculate_cache_dimensions(struct cache_t cache) {
  struct cache_addr_d addr_d;
  addr_d.offset = find_pow2(cache.block_size, 0);
  addr_d.set_index = find_pow2(
      cache.cache_size * 1024 / (cache.block_size * cache.associativity), 0);
  addr_d.tag = ADDR_SIZE - addr_d.offset - addr_d.set_index;
  return addr_d;
}

void print_cache_dimensions(struct cache_addr_d addr_d) {
  printf("Number of bits for Tag: %u\n", addr_d.tag);
  printf("Number of bits for Set Index: %u\n", addr_d.set_index);
  printf("Numeber of bits for Offset: %u\n", addr_d.offset);
}
// get instruction from file, uses file_reader functions
struct inst_t instruction_from_file(FILE *file) {
  struct inst_t instruction;
  const char *line = read_line(file);
  char access_type;
  int32_t address;
  uint32_t num_inst;
  sscanf(line, "%c %x %u", &access_type, &address, &num_inst);
  if (access_type == LOAD_INST) {
    instruction.access_type = LOAD;
  } else if (access_type == STORE_INST) {
    instruction.access_type = STORE;
  }
  instruction.address = address;
  instruction.num_inst = num_inst;
  return instruction;
}

// print out the read instruction, for testing purposes
void print_instruction(struct inst_t inst) {
  switch (inst.access_type) {
  case LOAD:
    printf("Instruction type: LOAD\n");
    break;
  case STORE:
    printf("Instruction type: STORE\n");
    break;
  default:
    printf("Couldn't match instruction type\n");
    break;
  }
  printf("Address: %x\n", inst.address);
  printf("Instructions since last memory access: %u\n", inst.num_inst);
}

void print_all_inst(FILE *file) {
  while (!feof(file)) {
    struct inst_t inst = instruction_from_file(file);
    print_instruction(inst);
  }
}
