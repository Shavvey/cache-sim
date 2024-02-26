# compiler of chocie plus the flags used during compilation
CC=gcc
CFLAGS=-W -Wall -g
DEPS = file_reader.h
OBJ = main.o file_reader.o

# make each object file from dependencies and sources
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cache-sim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# clean by deleting all object files and core files
.PHONY: clean
clean:
	rm -f $(OBJ) core


