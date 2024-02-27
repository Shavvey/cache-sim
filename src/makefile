# compiler of choice plus the flags used during compilation
CC=gcc
CFLAGS=-W -Wall -g
DEPS = file_reader.h cache.h trace.h
OBJ = main.o file_reader.o cache.o trace.o

# make each object file from dependencies and sources
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# make cache-sim final executable, put this inside the parent dir
cache-sim: $(OBJ)
	$(CC) -o ../cache-sim $^ $(CFLAGS)

# clean by deleting all object files and core files
.PHONY: clean
clean:
	rm -f $(OBJ) core

# test cache-sim program using sample config and trace
.PHONY: test
test: ../cache-sim
	../cache-sim ../sample.conf ../traces/gcc.trace

