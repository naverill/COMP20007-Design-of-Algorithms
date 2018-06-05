/* * * * * * *
 * Module for Assignment 2 of COMP20007 Design of Algorithms
 * Text spelling corrector
 *
 * Task 1:
 *		- Compute levenshtein distance between two words
 *
 * Task 2:
 *		- Enumerate all possible edits of a word
 *
 * Task 3:
 *		- Check text for incorrectly-spelled word
 *
 * Task 4:
 *		- Check text for incorrectly-spelled words and replace with a correctly
 *			spelled word with levenshtein distance of less than or equal to 3
 *
 *
 * created for COMP20007 Design of Algorithms
 * by Nina Averill <naverill@student.unimelb.edu.au>
 */
#define ALPHA_LEN 26
#define TOLERANCE 3
#define DICT_SIZE 250000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "spell.h"
#include "hashtbl.h"
#include "bktree.h"

const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

//general
char *substr(char *word, int start, int end);
char *concat(char *word1, char *word2);

// task 1
//get_edit_dist() and min() implementation located in bktree.c

// task 2
void print_all_insertions(char *word);
void print_all_deletions(char *word);
void print_all_substitutions(char *word);

// task 3
HashTable *create_dict(List *dictionary);

// task 4
BKTree *create_tree(List *dictionary);
char *get_closest_match(BKTree *bk_tree, char *word);

/**********************************Task 1***********************************/
// see Assignment Task 1: Computing edit distance
void print_edit_distance(char *word1, char *word2) {

	int edit_dist = get_edit_dist(word1, word2);

	printf("%d\n", edit_dist);
}

/**********************************Task 2***********************************/
// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word) {
	print_all_insertions(word);
	print_all_deletions(word);
	print_all_substitutions(word);
}

/**
* PRINT ALL EDITS OF `word` WITH LETTER INSERTED
*/
void print_all_insertions(char *word){
	int s = strlen(word);
	char *edit;
	char c[2] = {0, 0};

	//add letter from alphabet to word at index i
	int i, j;
	for(i=0; i<s+1; i++){
		for(j=0; j<ALPHA_LEN; j++){
			c[0] = alphabet[j];
			edit = concat(concat(substr(word, 0, i), (char *) &c),
							substr(word, i, s-i));

			printf("%s\n", edit);
			free(edit);
		}
	}
}

/**
* PRINT ALL EDITS OF `word` WITH LETTER DELETED
*/
void print_all_deletions(char *word){
	int s = strlen(word);
	char *edit;

	//remove letter from word at index i
	int i;
	for(i=0; i<s; i++){
		edit = concat(substr(word, 0, i), substr(word, i+1, s-(i+1)));

		printf("%s\n", edit);
		free(edit);
	}
}

/**
* PRINT ALL EDITS OF `word` WITH LETTER SUBSTITUTED
*/
void print_all_substitutions(char *word){
	int s = strlen(word);
	char *edit;
	char c[2] = {0, 0};

	//replace letter in word at index i
	int i, j;
	for(i=0; i<s; i++){
		for(j=0; j<ALPHA_LEN; j++){
			c[0] = alphabet[j];
			edit = concat(concat(substr(word, 0, i),  (char *) &c),
							substr(word, i+1, s -(i+1)));

			printf("%s\n", edit);
			free(edit);
		}
	}
}

/**********************************Task 3***********************************/
// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
	HashTable *dict = create_dict(dictionary);

	//iterate through words in document, checking if word is stored in HashTable
	//`dict`, i.e. is correctly spelled
	Node *curr;
	for(curr = document->head; curr!=NULL; curr = curr->next){
		char *word = curr->data;

		if(!word){
			continue;
		}

		if(hash_table_has(dict, word)){
			printf("%s\n", word);
		} else {
			printf("%s?\n", word);
		}
	}
	free_hash_table(dict);
}

/**
* CREATE A HASHTABLE FROM A DICTIONARY OF WORDS
*/
HashTable *create_dict(List *dictionary){
	HashTable *new_dict = new_hash_table(DICT_SIZE);

	//insert all words in `dictionary` into HashTable
	Node *curr;
	for(curr = dictionary->head; curr!=NULL; curr = curr->next){
		if(!curr->data){
			continue;
		}

		hash_table_put(new_dict, curr->data);
	}
	return new_dict;
}

/**********************************Task 4***********************************/
// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document) {
	HashTable *dict = create_dict(dictionary);
	BKTree *bk_tree = create_tree(dictionary);

	Node *curr;
	for(curr = document->head; curr!=NULL; curr = curr->next){

		char *word = curr->data;
		//skip over empty words in list
		if(!word){
			continue;
		}

		if(hash_table_has(dict, word)){
			printf("%s\n", word);

		} else {
			//find closest match with edit distance <= TOLERANCE
			char *match = get_closest_match(bk_tree, word);

			if(match){
				printf("%s\n",match);
			} else {
				printf("%s?\n", word);
			}
		}
	}
	//free data structures
	free_hash_table(dict);
	free_tree(bk_tree);
}

/**
* CREATE A BK TREE FROM A DICTIONARY OF WORDS
*/
BKTree *create_tree(List *dictionary){
	BKTree *new_tree = init_tree();

	//insert all words in `dictionary` into BK Tree
	Node *curr;
	for(curr=dictionary->head; curr!=NULL; curr=curr->next){
		if(!curr->data){
			continue;
		}

		insert(new_tree, curr->data);
	}

	return new_tree;
}

/**
* FIND CLOSEST VALID WORD IN BK TREE
*/
char *get_closest_match(BKTree *bk_tree, char *word){
	char *match=NULL;
	int ldist;
	//Search tree for words of Levenshtein distance <= ldist, returning the
	//first match found
	for(ldist=1; ldist<=TOLERANCE; ldist++){
		search(bk_tree->root, word, ldist, &match);

		if(match){
			return match;
		}
	}
	return NULL;
}


/**********************************General***********************************/
/* * * * * * *
 * Module for creating and manipulating substrings
 *
 * created for spelling_corrector by marcelotoledo
 * Edited by Nina Averill
 */

 /**
 * CREATE A SUBSTRING OF `word` FROM INDEX [offset, limit]
 */
char *substr(char *word, int offset, int limit){
	int len = strlen(word);

	if(limit>len || ((offset + limit)>len) || len<1 || limit == 0){
		return NULL;
	}

	char *sub = malloc((len+1) * sizeof(char));
	if(!sub){
		return NULL;
	}

	strncpy(sub, word + offset, limit);
	sub[limit] = '\0';

	return sub;
}

/**
* CONCATENATE TWO STRINGS
*/
char *concat(char *word1, char *word2){
	if (!word1) {
		word1 = malloc(sizeof(char));
		*word1 = '\0';
	}

	if (!word2) {
		word2 = malloc(sizeof(char));
		*word2 = '\0';
	}

	word1 = realloc(word1, strlen(word1) + strlen(word2) + 1);
	return strcat(word1, word2);
}
