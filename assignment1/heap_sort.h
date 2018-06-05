#ifndef HEAP_SORT_H
#define HEAP_SORT_H

//function to sort items in an array
void heap_sort(HeapItem h[],int n);
//Build a heap from the items in the array
void heapify(HeapItem h[],int n);
//Reconstruct heap remvoving the largest remaining item
void adjust(HeapItem h[],int n);

#endif
