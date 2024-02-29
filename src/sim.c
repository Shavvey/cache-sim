#include "cache.h"
#include "file.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BLOCK_SIZE sizeof(struct blocks_t *)
#define SET_SIZE sizeof(struct set_t *)
// GLOBALS
// cache configuration
struct cache_t cache_conf;
// cache address dimensions
struct cache_addr_d addr_d;
uint32_t num_sets;
uint32_t num_blocks;
// trackers for hits and misses
int TOTAL_HITS = 0;
int TOTAL_MISSES = 0;
int LOAD_MISS = 0;
int STORE_MISS = 0;
int LOAD_HIT = 0;
int STORE_HIT = 0;
int NUM_OTHER_INST = 0;
int MEM_INST_READ = 0;
// track evicts too
int EVICTS = 0;
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

// record hit  depending on what type it is
void record_hit(enum access_type type) {
  if (type == LOAD) {
    LOAD_HIT++;
  } else {
    STORE_HIT++;
  }
}
// record miss depending on what type it is
void record_miss(enum access_type type) {
  if (type == LOAD) {
    LOAD_MISS++;
  } else {
    STORE_MISS++;
  }
}

// helper function for bit mask, just return the power using integer bases and
// exponents
int power_rec(int base, int exponent) {
  return (exponent == 1) ? base : base * power_rec(base, exponent - 1);
}
// make a bit mask based on the number of bits needed and the offset
uint32_t make_bit_mask(uint8_t num_bits, uint8_t offset) {
  uint32_t mask = power_rec(2, num_bits) - 1;
  return mask << offset;
}
// compare two bits of a given address
bool compare_tag(struct cache_addr_d addr, uint32_t address1,
                 uint32_t address2) {
  uint32_t mask = make_bit_mask(addr.tag, addr.offset);
  // compare the bits using the mask
  return ((address1 & mask) == (address2 & mask));
}
uint32_t get_set_bits(struct cache_addr_d addr, uint32_t address) {
  uint32_t mask = make_bit_mask(addr.set_index, addr.offset);
  address = (address & mask);
  return (address >> addr.offset);
}
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
struct blocks_t *create_block(uint32_t addr) {
  struct blocks_t *block = (struct blocks_t *)malloc(BLOCK_SIZE);
  if (block != NULL) {
    // set link to be null explicitly
    block->next = NULL;
    block->addr = addr;
  } else {
    perror("Error allocating memory for block");
  }
  return block;
}
// handle cache eviction if the set is already at capacity
void handle_eviction(struct set_t *set, enum rep_policy policy, uint32_t addr) {
  struct blocks_t *block = create_block(addr);
  int r;
  EVICTS++;
  switch (policy) {
    // execute the FIFO eviction policy (remove first cache block and insert new
    // cache block at the end)
  case FIFO:
    // add old head's next pointer to the block pointer
    block->next = set->blocks;
    set->blocks = block;
    // use head and prev pointers to remove from linked list
    struct blocks_t *head = set->blocks;
    struct blocks_t *prev;
    // remove the last block
    while (head->next != NULL) {
      prev = head;
      // goto next block
      head = head->next;
    }
    prev->next = NULL;
    // the last block inside linked list
    free(head);
    break;
    // execute random eviction policy
  case RANDOM:
    // return random value between 0 and associativity of cache - 1
    r = rand() % cache_conf.associativity;
    // use tempt var to get head of the linked list of cache blocks
    struct blocks_t *chosen = set->blocks;
    prev = chosen;
    for (int i = 0; i < r; i++) {
      prev = chosen;
      // goto next element in linked list
      chosen = chosen->next;
    }
    // if we are point to head (for loop is not ran)
    if (chosen == prev) {
      // inherit current heads link
      block->next = chosen->next;
      // set new block to be head
      set->blocks = block;
      // free chosen block, in this case the head
      free(chosen);
    } else if (chosen->next == NULL) {
      // otherwise this is just the tail
      prev->next = block;
      free(chosen);
      // else we need to conjoined the two ends of the linked list and free
      // the picked node
    } else {
      // final case we're we are in the middle somewhere
      // create local var to store next of chosen block, just for clarity
      struct blocks_t *next = chosen->next;
      // previous block now links to new block
      prev->next = block;
      // inherit chosen's block link
      block->next = next;
      // free chosen node
      free(chosen);
    }
    break;
  }
}
// search a set for a given addhandle bytes erroresress by looping through each
// block
bool search_set(struct set_t *set, uint32_t addr) {
  struct blocks_t *head = set->blocks;
  while (head != NULL) {
    // compare tag of given instruction address with given block address in set
    if (compare_tag(addr_d, addr, set->blocks->addr)) {
      // NOTE: just for testing purposes
      // printf("Tag matched!\n");
      return 1;
    }
    // traverse linked list of blocks
    head = head->next;
  }
  // otherwise return false
  return 0;
}
void handle_miss(struct set_t *set, struct cache_t cache_conf, uint32_t addr) {
  if (set->blocks == NULL) {
    set->blocks = create_block(addr);
    set->length++;
    // early return since blocks are already empty
    return;
  }
  // NO EVICTION POLICY IS NEEDED
  if (set->length < cache_conf.associativity) {
    struct blocks_t *head = set->blocks;
    // insert new block into list
    while (head->next != NULL) {
      // traverse link list until we reach the end
      head = head->next;
    }
    // create new block
    head->next = create_block(addr);
    // increment length, since we just adding another cache block to the set
    set->length++;
  } else {
    handle_eviction(set, cache_conf.rep_policy, addr);
  }
}
// check to see if the policy allows current instruct type to be allocated
bool check_policy(enum write_alloc policy, enum access_type type) {
  // if the write policy allos
  if (policy == NO && type == STORE) {
    return false;
  } else {
    return true;
  }
}
void write_to_textfile(char *trace_fname) {
  int MISS_PENALTY = cache_conf.miss_penalty;
  // calculate the total hit rate
  float TOTAL_HIT_RATE = ((float)(STORE_HIT + LOAD_HIT) / MEM_INST_READ) * 100;
  float TOTAL_MISS_RATE =
      ((float)(STORE_MISS + LOAD_MISS) / MEM_INST_READ) * 100;
  // calculate percentage of load hits
  float LOAD_HIT_RATE = ((float)LOAD_HIT / MEM_INST_READ) * 100;
  // calcualte percentage of stoer hits
  float STORE_HIT_RATE = ((float)STORE_HIT / MEM_INST_READ) * 100;
  // the total run time in cycles (assume all other instructions have a CPI of 1
  // and that hit time is one cycle)
  uint32_t TOTAL_RUN_TIME = (STORE_HIT + LOAD_HIT) +
                            (STORE_MISS + LOAD_MISS) * cache_conf.miss_penalty +
                            NUM_OTHER_INST;
  // average memory latency is (hit_time) + (miss_rate)*(miss_time)
  // NOTE:
  // assumes hit time is one cycle
  float AVERAGE_MEM_LATENCY =
      1 + ((float)MISS_PENALTY / 100 *
           TOTAL_MISS_RATE); // inverse of hit rate is miss rate
  // get the size of the trace name
  int size = strlen(trace_fname);
  char name[size];
  // copy string literal of trace name into new mutable character array
  strcpy(name, trace_fname);
  create_output_fname(name, ".out");
  char *f = name;
  // strip path info inside the name if there is any
  char *fname = strip_path_info(f);
  file = get_file(fname, "w");
  // begin writing information to the textfile
  char buffer[50];
  snprintf(buffer, 50, "The total hit rate is: %f %%\n", TOTAL_HIT_RATE);
  write_line(file, buffer);
  snprintf(buffer, 50, "The load hit rate is: %f %%\n", LOAD_HIT_RATE);
  write_line(file, buffer);
  snprintf(buffer, 50, "The store hit rate is: %f %%\n", STORE_HIT_RATE);
  write_line(file, buffer);
  snprintf(buffer, 50, "The total run time is: %u cycles\n", TOTAL_RUN_TIME);
  write_line(file, buffer);
  snprintf(buffer, 50, "The average memory access latency: %f cycles\n",
           AVERAGE_MEM_LATENCY);
  write_line(file, buffer);
  // print out the statistics to stdout because why not
  printf("The total hit rate is: %f%%\n", TOTAL_HIT_RATE);
  printf("The load hit rate is: %f%%\n", LOAD_HIT_RATE);
  printf("The store hit rate is; %f%%\n", STORE_HIT_RATE);
  printf("The total run time is: %u cycles\n", TOTAL_RUN_TIME);
  printf("The average memory access latency: %f cycles\n", AVERAGE_MEM_LATENCY);
  // close the file after writing the lines
  fclose(file);
}

// after loading in cache config we can finally perform the cache simulation
void cache_sim(char *trace_fname) {
  // init random seed (used for random cache eviction policy)
  srand((unsigned int)time(NULL));
  // declare array of sets to represent cache, based on number of set obtained
  // from cache config
  // create cache struct that represents the state of the cache
  struct cache cache[num_sets];
  // allocate memory for each set, leave the block pointer set to null for now
  for (uint32_t i = 0; i < num_sets; i++) {
    cache[i].set = (struct set_t *)malloc(SET_SIZE);
    if (cache[i].set != NULL) {
      // set the length of the set to be zero explicitly
      cache[i].set->length = 0;
      // set the pointer to the chain of blocks to NULL explicitly
      cache[i].set->blocks = NULL;
    }
  }
  FILE *trace_file = get_file(trace_fname, "r");
  int flag = 1;
  // whiles bytes have been successfully read and the file has not ended, keep
  // running the cache sim loop
  while (flag && !feof(trace_file)) {
    // get the instruction from the trace file
    struct inst_t inst = instruction_from_file(trace_file);
    // record how other instructions were executed that we're not memory
    // references
    NUM_OTHER_INST += inst.num_inst;
    // **PROCESSING THE INSTRUCTION**
    // if read actually returns some bytes, then process the read instruction
    flag = (bytes > 0);
    if (flag) {
      MEM_INST_READ++;
      uint32_t set_bits = get_set_bits(addr_d, inst.address);
      // use set bits to index into the cache
      struct set_t *set = cache[set_bits].set;
      // search the set for the given tag and decoded instruction
      bool in_set = search_set(set, inst.address);
      // check to see if there is write allocate
      bool is_write = check_policy(cache_conf.write_alloc, inst.access_type);
      // cache hit! increment hit counter
      if (in_set) {
        record_hit(inst.access_type);
      } else if (is_write) {
        record_miss(inst.access_type);
        // handle the miss, either eviction or creation of new cache block
        handle_miss(set, cache_conf, inst.address);
      } else {
        // record miss since policy doesn't allow use to a block for writes
        // we do nothing else
        record_miss(inst.access_type);
      }
    }
  }
  fclose(file);
  printf("INSTRUCTIONS READ: %d\n", MEM_INST_READ);
  printf("HITS: %d\n", STORE_HIT + LOAD_HIT);
  printf("MISSES: %d\n", STORE_MISS + LOAD_MISS);
  printf("LOAD MISSES: %d\n", LOAD_MISS);
  printf("STORE MISSES: %d\n", STORE_MISS);
  printf("STORE HITS: %d\n", STORE_HIT);
  printf("LOAD HITS: %d\n", LOAD_HIT);
  printf("NUMBER OF OTHER EXECUTED INSTRUCTIONS: %d\n", NUM_OTHER_INST);
  printf("EVICTS: %d\n", EVICTS);
  write_to_textfile(trace_fname);
}
