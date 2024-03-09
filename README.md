# CSE331 Lab 2 - Cache Simulator

## Project Description

This program implements a very simple, trace-driven cache simulator. The executable takes two arguments:

- a config file path, relative to executable
- a trace file path, relative to executable

### Config File

The config file is a textfile that contains some basic design information about the cache. The choices for the cache design include the following:

- _Cache Line\Block Size_: describes the size of each block\line inside the cache, should be some power of 2.
- _N-way Associativity_: describes how many blocks are placed withing the sets of a cache. In a direct-mapped cache,
  the associativity is simple set to 1.
- _Cache\Data Size_: the total cache size, in kilobytes, of the cache.
- _Replacement Policy_: the eviction/replacement policy that is invoked during a conflict miss,
  when a victim cache is replaced with a new cache block.
  Currently the two implemented policies are _FIFO_(0) and _RANDOM_(1).
- _Miss Penalty_: the penalty, measured in cycles for a cache miss. Can be any integer.
- _Write Allocate_: Policies for write blocks: can either be _no-write-allocate_ do not allocate memory in cache for writes,
  or _write-allocate_ where memory is allocated inside the cache for writes.

**We assume that write-allocate caches are write-through, and no-write-allocate are write back caches**
The configuration file we will be read line-by-line to construct the design of the cache.

### Trace File

The trace file is textfile with a seqeuence of memory references
(which is a 32-bit address) and then an access type is specified, and also how many instructions we executed
between memory references.
**For simplicity, we assume that all instructions that are not memory references simply have a cycle time of 1**

Here's an example of an instruction trace inside a trace file:
`s 0x1fffff50 1`, `s` specifies a store reference, `0x1fffff50` is the address, and `1`
the number of other instructions types that have been executed after the last memory reference.

## Project Implementation

The cache simulator uses a couple C source files:

- _main.c_: contains the entry point into the program, all it's responsible for it loading the command args and
  executing two function `load_cache_config` to create the cache config, and `cache-sim` to run the sim, that's all.
- _file.c_: responsible for file IO operations, two big ones are `get_file`, which opens a file a returns the file pointer
  and `get_line` which uses the file pointer to read a line of text from the file.
- _cache.c_: uses `get_cache_config` to return a struct `cache_t` that represents the type of the cache.
- _sim.c_: runs the simulator, and is by far the biggest source file. The sim loop very basically involves
  reading a instruction from the trace using `instructino_from_file`, getting sets bits from address in trace using
  `get_set_bits`, then searching the set via tag comparison using `search_set`, checking write-alloc
  policies using `check_policy`, then recording a hit if cache block is found,
  and recording and handling a miss if it is not.

## Running the Project

The project includes a `makefile` that can compile the project using gcc into an executable called `cache-sim`.
To use the make file, you should use the following commands inside the
main directory (the directory with the README and makefile).

```sh
make # make project into final executable
make test # test exec using sample.conf and
```

the `make test` rule will use the exec to load in the `sample.conf` and `gcc.trace` as sample output for the program.
