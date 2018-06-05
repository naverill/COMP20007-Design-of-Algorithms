# # # # # # #
# Makefile for assignment 2
#
# created May 2018
# Matt Farrugia <matt.farrugia@unimelb.edu.au>
#

CC     = gcc
CFLAGS = -Wall -std=c99
# modify the flags here ^
EXE    = a2
OBJ    = src/main.o src/list.o src/spell.o src/hashtbl.o src/strhash.o src/bktree.o
# add any new object files here ^

# top (default) target
all: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# other dependencies
src/main.o: src/list.h src/spell.h src/hashtbl.h
src/spell.o: src/spell.h src/list.h src/bktree.h
src/list.o: src/list.h
src/hashtbl.o: src/hashtbl.h src/strhash.h
src/strhash.o: src/strhash.h
src/bktree.o: src/bktree.h src/list.h src/spell.h
# ^ add any new dependencies here (for example if you add new modules)

# phony targets (these targets do not represent actual files)
.PHONY: clean cleanly all CLEAN

# `make clean` to remove all object files
# `make CLEAN` to remove all object and executable files
# `make cleanly` to `make` then immediately remove object files (inefficient)
clean:
	rm -f $(OBJ)
CLEAN: clean
	rm -f $(EXE)
cleanly: all clean
