/* * * * * * *
 * Module for creating and manipulating binary min-heaps on integer indices
 *
 * created for COMP20007 Design of Algorithms 2013
 * by Andrew Turpin
 * modified by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * modified by Nina Averill <naverill@student.unimelb.edu.au>
 */

typedef struct heap Heap;

typedef struct item {
	void *key;  // the key for deciding position in heap
	void *data;  // the data value associated with this key
} HeapItem;

struct heap {
	HeapItem *items;  // the underlying array
	int cur_size; // the number of items currently in the heap
	int max_size; // the maximum number of items allowed in the heap
  int (*cmp)(void*, void*);
};


// returns a pointer to a new, empty heap with a capacity for max_size items
Heap *new_heap(int max_size, int func(void*, void*));

// inserts data into h with priority key
void heap_insert(Heap *h, void *key, void *data);

// remove and return the item with the smallest key in h
void *heap_remove_min(Heap *h);

// return, but not remove, the item with the smallest key in h
void *heap_peek_min(Heap *h);

// return the key of the item with the smallest key in h
void *heap_peek_key(Heap *h);

// de-allocate all memory used by h
void free_heap(Heap *h);

void siftup(Heap *h, int i);

void siftdown(Heap *h, int index);
