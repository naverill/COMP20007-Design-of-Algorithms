/* * * * * * *
 * Module for creating and manipulating binary min-heaps on integer indices
 *
 * created for COMP20007 Design of Algorithms 2013
 * by Andrew Turpin
 * modified by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * modified by Nina Averill <naverill@student.unimelb.edu.au>
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"

// HELPER FUNCTIONS

// swap two heap items (the items at index i and index j)
// assumes h != NULL
void swap(Heap *h, int i, int j) {
	HeapItem t  = h->items[i];
	h->items[i] = h->items[j];
	h->items[j] = t;
}

// move an item up the heap structure to re-establish heap order
// bonus challenge: improve this function by avoiding some copies/swaps
// assumes h != NULL
void siftup(Heap *h, int i) {
	int parent = (i - 1) / 2;
	while (h->cmp(h->items[i].key, h->items[parent].key)<0) {
		swap(h, parent, i);
		i = parent;
		parent = (i - 1) / 2;
	}
}

// find the index of the smallest child among the children of an item
// if the item has no children, return 0
// assumes h != NULL
int min_child(Heap *h, int i) {
	int child = i * 2 + 1;
	if (child >= h->cur_size) {
		// no children
		return 0;
	} else if (child+1 >= h->cur_size || h->cmp(h->items[child].key, h->items[child+1].key)<0) {
		// only child or first child is smallest child
		return child;
	} else {
		// second child exists and is smallest child
		return child+1;
	}
}

// move an item down the heap structure to re-establish heap order
// bonus challenge: improve this function by avoiding some copies/swaps
// assumes h != NULL
void siftdown(Heap *h, int index) {
	int child = min_child(h, index);
	while (child && h->cmp(h->items[child].key, h->items[index].key)<0) {
		swap(h, child, index);
		index = child;
		child = min_child(h, index);
	}
}

// returns a pointer to a new, empty heap with a capacity for maximum_size items
Heap *new_heap(int max_size, int func(void*, void*)) {
	Heap *h = malloc(sizeof *h);
	assert(h);

	h->items = malloc(max_size *(sizeof *h->items));
	assert(h->items);

	h->cur_size = 0;
	h->max_size = max_size;
	h->cmp = func;

	return h;
}

// inserts data into heap with priority key
void heap_insert(Heap *h, void *key, void *data) {
	assert(h != NULL);
	assert(h->cur_size < h->max_size);

	h->items[h->cur_size].key  = key;
	h->items[h->cur_size].data = data;
	h->cur_size++;

	siftup(h, h->cur_size-1);
}

// remove and return the item with the smallest key in h
void *heap_remove_min(Heap *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	void *min = h->items[0].data;

	h->items[0] = h->items[h->cur_size-1];
	h->cur_size -= 1;
	siftdown(h, 0);

	return min;
}

// return, but not remove, the item with the smallest key in h
void *heap_peek_min(Heap *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].data;
}

// return the key of the item with the smallest key in h
void *heap_peek_key(Heap *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].key;
}

// de-allocate all memory used by h
void free_heap(Heap *h) {
	assert(h != NULL);
	if(h->items){
		free(h->items);
	}
	free(h);
}
