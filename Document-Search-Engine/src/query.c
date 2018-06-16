/* * * * * * *
 * Module to find the
 *
 * created for COMP20007 Design of Algorithms - Assignment 1, 2018
 * by Nina Averill <nina.averill@student.unimelb.edu.au> */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "query.h"
#include "list.h"
#include "heap.h"
#include "heap_sort.h"

//Task 1
float *array_init(int n_documents);
void get_doc_scores(Index *index, float *doc_scores, int num_documents);
Heap *top_k_array_results(float *doc_scores, int n_results, int n_documents);
void print_top_k(Heap *top_k);

//Task 2
Heap *heap_init(int size);
void get_doc_lists(Index *index, Heap *doc_heap);
Heap *top_k_merge_results(Heap *doc_heap, int n_results);
void insert_top_k(Heap *top_k, float doc_score, int doc_id);
void rearrange_doc_heap(Heap *doc_heap, List *doc_list);

void free_heap_items(Heap *h);
int cmp_doc(void* x1, void* x2);
int cmp_list(void *x1, void *x2);


/**********************************Task 1***********************************/
//TODO consider edge cases

// see Assignment spec: 'Task 1: Array-based accumulation approach'
void print_array_results(Index *index, int n_results, int n_documents) {

	float *doc_scores = array_init(n_documents);

	//find total scores for each document
	get_doc_scores(index, doc_scores, n_documents);

	//Return heap containing id of top k documents and scores
	Heap *top_k = top_k_array_results(doc_scores, n_results, n_documents);

	print_top_k(top_k);

	//free heap items, heap and array of scores
	free_heap_items(top_k);
	free_heap(top_k);
	free(doc_scores);
}

//create a new, empty array of n_document scores
float *array_init(int n_documents){
	float *empty_array = (float *)malloc(sizeof(float)*n_documents);
	assert(empty_array);

	int i;
	for(i=0; i<n_documents; i++){
		empty_array[i] = 0.0;
	}

	return empty_array;
}

//Create a total score for each document, by summing up the document score for
// each inidvidual term
void get_doc_scores(Index *index, float *doc_scores, int num_documents){
	int i;
	//for each list in index
	for(i=0; i<index->num_terms; i++){
		List *doclist = (List*)index->doclists[i];
		Node *cur_doc = doclist->head;

		//for each document in list, sum up scores
		while(cur_doc){
			doc_scores[((Document*)cur_doc->data)->id] +=
												((Document*)cur_doc->data)->score;
			cur_doc = cur_doc->next;
		}
	}
}

//Create heap of top k documents that have the highest score
Heap *top_k_array_results(float *doc_scores, int n_results, int n_documents){
	//initialise heap with appropriate key comparison function and n-results
	//elements
	int (*cmp)(void*, void*);
  cmp = &cmp_doc;
	Heap *top_k = new_heap(n_results, cmp);

	//populate heap with first n_results scores
	int doc;
	for(doc=0; doc<n_results; doc++){
		  int *doc_num = (int*)malloc(sizeof(int));
			float *doc_score = (float*)malloc(sizeof(float));
			*doc_num = doc;
			*doc_score = doc_scores[*doc_num];

			heap_insert(top_k, doc_score, doc_num);
	}

	//for n_results...n_document scores
	for(doc=n_results; doc<n_documents; doc++){
		int *doc_num = (int*)malloc(sizeof(int));
		float *doc_score = (float*)malloc(sizeof(float));

		*doc_num = doc;
		*doc_score = doc_scores[*doc_num];

		//if doc score is greater than min in heap, replace min with current doc
		if(*doc_score > *(float*)heap_peek_key(top_k)){
			heap_remove_min(top_k);
			heap_insert(top_k, doc_score, doc_num);
		}
	}

	return top_k;
}

void print_top_k(Heap *top_k){
	assert(top_k != NULL);
	//sort items in descending order or scores
	heap_sort(top_k->items, top_k->cur_size);

	int i;
	//for each
	for(i=0; i<top_k->cur_size; i++){
		int id = *(int*)top_k->items[i].data;
		float score = *(float*)top_k->items[i].key;

		//only print non-zero scores
		if(score>0){
			printf("%6d %.6f\n", id, score);
		}
	}
}

/**********************************Task 2***********************************/

// see Assignment spec: 'Task 2: Priority queue-based multi-way merge approach'
void print_merge_results(Index *index, int n_results) {
	//create empty heap of document lists
	Heap *doc_heap = heap_init(index->num_terms);

	//populate heap
	get_doc_lists(index, doc_heap);
	//Return heap containing id of top k documents and scores
	Heap *top_k = top_k_merge_results(doc_heap, n_results);

	print_top_k(top_k);

	//free heap items and heaps
	free_heap_items(top_k);
	free_heap(top_k);
	free_heap(doc_heap);
}

//create heap with size elements
Heap *heap_init(int size){
	//initialise with appropriate comparison function
	int (*cmp)(void*, void*);
	cmp = &cmp_list;
	return new_heap(size, cmp);
}

//Populate empty heap, with data = list of documents and
//keys = id of the first document in the list
void get_doc_lists(Index *index, Heap *doc_heap){
	int i;
	//for each term in list
	for(i=0; i<index->num_terms; i++){
		heap_insert(doc_heap, &((Document*)index->doclists[i]->head->data)->id,
														(List*)index->doclists[i]);
	}
}

//Find top k scores by iterating through ordered lists of document, only
//looking at first document in the list. Once all instances of the document
//have been found, update heap with document score
Heap *top_k_merge_results(Heap *doc_heap, int n_results){
	//initalise dheap with approprate comparison function
  int (*cmp)(void*, void*);
  cmp = &cmp_doc;
	Heap *top_k = new_heap(n_results, cmp);


	Document *doc;
	//while there are non-empty lists in doc_heap
	while(doc_heap->cur_size > 0){
		//find list with smallest id of the first document
		List *doc_list = (List*)heap_peek_min(doc_heap);

    Document *cur_doc = doc_list->head->data;

		//initialise doc with first in list document
		if(!doc){
			doc = new_document(cur_doc->id, cur_doc->score);;

		//If all instances of the document has been found in the lists,
		//update heap
		} else if (doc->id != cur_doc->id){

			//Update heap with previous document score
			insert_top_k(top_k, doc->score, doc->id);

			free(doc);
			//update doc with current doc score
			doc = new_document(cur_doc->id, cur_doc->score);

		//Else cur_doc has the same id as doc, therefore update score
		} else {
			doc->score += cur_doc->score;
		}

		//remove id from current list
		list_remove_start(doc_list);
		//rearrance lists in order of the smallest id of the first document
		rearrange_doc_heap(doc_heap, doc_list);

	}
	free(doc);
	return top_k;
}

//Update heap with document score
void insert_top_k(Heap *top_k, float doc_score, int doc_id) {
	int *id = malloc(sizeof(int));
	float *score = malloc(sizeof(float));
	assert(id && score);
	*id = doc_id;
	*score = doc_score;

	//popuate heap for first  0...max documents
	if(top_k->cur_size < top_k->max_size){
		heap_insert(top_k, score, id);
	//if document found with score higher than the minimum in heap, update heap
	} else {
		if(*score > *(float*)heap_peek_key(top_k)){
			heap_remove_min(top_k);
			heap_insert(top_k, score, id);
		} else {
			//free document
			free(id);
			free(score);
		}
	}
}

//Rearrange heap of document lists such that they are in order of smallest
// id of the first document
void rearrange_doc_heap(Heap *doc_heap, List *doc_list){
	//if the list is non-empty
	if(!list_is_empty(doc_list)){
		//update key of min list in heap, with new first doc id in list,
		// and rearrange heap
		doc_heap->items[0].key = &(((Document*)doc_list->head->data)->id);
		siftdown(doc_heap, 0);
	//remove empty list
	} else {
		heap_remove_min(doc_heap);
	}
}

//free all allocated memory used in heap
void free_heap_items(Heap *h){
	int i;
	//for each
	for(i=0; i<h->cur_size; i++){
		int *temp_data = (int*)h->items[i].data;
		float *temp_key = (float*)h->items[i].key;

		h->items[i].data = NULL;
		h->items[i].key = NULL;
		//free HeapItem
		free(temp_data);
		free(temp_key);
	}
}

//comparison function for heap of documents, where key is a float containing
//the score of the document
int cmp_doc(void* x1, void* x2){
	assert(sizeof(x1)==sizeof(x2) && sizeof(x2)==sizeof(float*));

	if(*(float*)x1 - *(float*)x2 < 0){
		return -1;
	} else {
		return 1;
	}
}

//comparison function for heap of document lists, where key is an int
//containing the id of first document in the list
int cmp_list(void *x1, void *x2){
	assert(sizeof(x1)==sizeof(x2) && sizeof(x2)==sizeof(int*));
  return *((int*)x1) - *((int*)x2);
}
