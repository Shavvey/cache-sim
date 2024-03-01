# compiler of choice plus the flags used during compilation
CC=gcc
CFLAGS=-W -Wall -g
DEPS = src/file.h src/cache.h src/sim.h
OBJ = src/main.o src/file.o src/cache.o src/sim.o

# make each object file from dependencies and sources
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# make cache-sim final executable, put this inside the parent dir
cache-sim: $(OBJ)
	$(CC) -o cache-sim $^ $(CFLAGS)

# clean by deleting all object files and core files
.PHONY: clean
clean:
	rm -f $(OBJ) core

# test cache-sim program using sample config and trace
.PHONY: test
test: ../cache-sim
	../cache-sim ../sample.conf ../traces/gcc.trace

