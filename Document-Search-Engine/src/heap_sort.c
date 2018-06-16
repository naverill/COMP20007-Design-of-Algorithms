/* * * * * * *
 * Module for sorting an array of HeapItems
 *
 * Created by scanftree.com,
 * Modified by Nina Averill <naverill@sudent.unimelb.edu.au>
 */
#include <stdlib.h>
#include "heap.h"
#include "heap_sort.h"

//function to sort items in an array
void heap_sort(HeapItem h[], int n) {
	int i;
  HeapItem item;
	//build heap
	heapify(h,n);
	for (i=n-1; i>0; i--) {
		//move current root to end
		item = h[0];
		h[0] = h[i];
		h[i] = item;
		adjust(h, i);
	}
}

//Build a heap from the items in the array
void heapify(HeapItem h[],int n) {
	int k,i,j;
	HeapItem item;
	//for each element in array
	for (k=1; k<n; k++) {
		item = h[k];
		i = k;
		j = (i - 1)/2;

		// While current item has smaller key than child
		//swap parent and child
		while((i>0) && (*(float*)item.key < *(float*)h[j].key)) {
			h[i] = h[j];
			i = j;
			j = (i-1)/2;
		}
		h[i] = item;
	}
}

//Reconstruct heap remvoving the largest remaining item
void adjust(HeapItem h[],int n) {
	int i,j;
  HeapItem item;
	j = 0;
	item = h[j];
	i = 2 * j + 1;
	while(i<=n-1) {
		if(i+1 <= n-1){
		   if(*(float*)h[i].key > *(float*)h[i+1].key){
		    i++;
      }
    }
		if(*(float*)item.key > *(float*)h[i].key) {
			h[j] = h[i];
			j = i;
			i = 2*j+1;
		} else{
		   break;
     }
	}
	h[j] = item;
}
