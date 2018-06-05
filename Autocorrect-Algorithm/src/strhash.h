/* * * * * * * * * * *
 * Module providing several hash functions for strings
 *
 * created for COMP20007 Design of Algorithms
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * modified by Nina Averill
 *
 * available hash functions:
 *   'x'  | near-universal hash function based on xoring and bit shifting
 */


// generate a hash value for key (a string) to a hash table of size entries,
// using hash method 'method'
// guarantees a return value between 0 (inclusive) and size (non inclusive)
unsigned int hash(const char *key, unsigned int size);

// return the string name of a hash function specified by method
char *name(char method);
