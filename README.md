# CSE331 Lab 2 - Cache Simulator

## Project Description

This program implements a very simple, trace-driven cache simulator. The executable takes two arguments:

- a config file path, relative to executable
- a trace file path, relative to executable

## Config File

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

## Trace File

The trace file is textfile with a seqeuence of memory references
(which is a 32-bit address) and then an access type is specified
