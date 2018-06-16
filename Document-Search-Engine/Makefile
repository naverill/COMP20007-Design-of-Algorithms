# # # # # # #
# Makefile for assignment 1
#
# created Mar 2018
# Matt Farrugia <matt.farrugia@unimelb.edu.au>
#

CC     = gcc
CFLAGS = -Wall -std=c99
EXE    = a1
OBJ    = src/main.o src/query.o src/index.o src/list.o src/heap.o src/heap_sort.o
#				add any new object files here ^

# top (default) target
all: $(EXE)

# how to link executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# other dependencies
src/main.o: src/query.h src/index.h
src/query.o: src/query.h src/index.h src/list.h src/heap.h src/heap_sort.h
src/index.o: src/index.h src/list.h
src/list.o: src/list.h
src/heap.o: src/heap.h
src/heap_sort.o: src/heap_sort.h src/heap.h

# ^ add any new dependencies here

# phony targets (do not represent actual files)
.PHONY: clean cleanly all CLEAN
# `make clean` to remove all object files
# `make CLEAN` to remove all object and executable files
# `make cleanly` to `make` then immediately remove object files (inefficient)
clean:
	rm -f $(OBJ)
CLEAN: clean
	rm -f $(EXE)
cleanly: all clean
