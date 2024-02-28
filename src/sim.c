#include "cache.h"
#include "file_reader.h"
#include "trace.h"
#include <stdlib.h>
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
uint32_t HITS = 0;
uint32_t MISSES = 0;
// track evicts too
uint32_t EVICTS = 0;
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
void handle_eviction(struct set_t *set, enum REP_POLICY policy, uint32_t addr) {
  struct blocks_t *block = create_block(addr);
  switch (policy) {
    // execute the FIFO eviction policy (remove first cache block and insert new
    // cache block at the end)
  case FIFO:
    EVICTS++;
    printf("FIFO Invoked!\n");
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
    EVICTS++;
    // return random value between 0 and associativity of cache - 1
    int r = rand() % cache_conf.associativity;
    printf("Random val: %d\n", r);
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
      printf("Tag matched!\n");
      return 1;
    }
    // traverse linked list of blocks
    head = head->next;
  }
  // otherwise return false
  return 0;
}
void handle_miss(struct set_t *set, struct cache_t cache_conf, uint32_t addr) {
  printf("Handle miss!\n");
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
    // increment length
    set->length++;
  } else {
    printf("Length: %d\n", set->length);
    handle_eviction(set, cache_conf.rep_policy, addr);
  }
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
  int read = 0;
  int flag = 1;
  while (flag && !feof(trace_file)) {
    // get the instruction from the trace file
    struct inst_t inst = instruction_from_file(trace_file);
    // **PROCESSING THE INSTRUCTION**
    // if read actually returns some bytes, then process the read instruction
    flag = (bytes > 0);
    if (flag) {
      read++;
      uint32_t set_bits = get_set_bits(addr_d, inst.address);
      // use set bits to index into the cache
      struct set_t *set = cache[set_bits].set;
      // search the set for the given tag and decoded instruction
      bool in_set = search_set(set, inst.address);
      // cache hit! increment hit counter
      if (in_set) {
        HITS++;
      } else {
        // cache miss! increment miss counter and handle error
        MISSES++;
        // handle the miss, either eviction or creation of new cache block
        handle_miss(set, cache_conf, inst.address);
      }
    }
  }
  printf("Lines read: %d\n", read);
  printf("HITS: %d\n", HITS);
  printf("MISSES: %d\n", MISSES);
  printf("EVICTS: %d\n", EVICTS);
  print_cache_dimensions(addr_d);
}
