#include <stdlib.h>
#include "heap.h"
#include "heap_sort.h"

void heap_sort(HeapItem h[],int n) {
	int i;
  HeapItem item;
	heapify(h,n);
	for (i=n-1; i>0; i--) {
		item = h[0];
		h[0] = h[i];
		h[i] = item;
		adjust(h, i);
	}
}
void heapify(HeapItem h[],int n) {
	int k,i,j,item;
	for (k=1; k<n; k++) {
		item = h[k];
		i = k;
		j = (i - 1)/2;

		while((i>0) && (*(float*)item.key > *(float*)h[j].key)) {
			h[i] = h[j];
			i = j;
			j = (i-1)/2;
		}
		h[i] = item;
	}
}
void adjust(HeapItem h[],int n) {
	int i,j
  HeapItem item;
	j = 0;
	item = h[j];
	i = 2 * j + 1;
	while(i<=n-1) {
		if(i+1 <= n-1){
		   if(*(float*)h[i].key < *(float*)h[i+1].key){
		    i++;
      }
    }
		if(item < h[i]) {
			h[j] = h[i];
			j = i;
			i = 2*j+1;
		} else{
		   break;
     }
	}
	h[j] = item;
}
