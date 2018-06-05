/* * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * edited by Nina Averill <naverill@student.unimelb.edu.au>
 */

#include <stdlib.h>
#include <string.h>
#include "strhash.h"

// xor hash from lectures, with seed 73802
unsigned int seed = 73802;
unsigned int xor_hash(const char *key, unsigned int size) {
	unsigned int h = seed;

	int i;
	for (i = 0; key[i] != '\0'; i++) {
		h = h ^ ((h << 5) + key[i] + (h >> 2));
	}

	return h % size;
}

// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size) {
	return xor_hash(key, size);
}
