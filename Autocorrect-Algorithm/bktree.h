/* * * * * * *
 * Module for creating, manipulating and searching BK Trees containing strings
 *
 * created for COMP20007 Design of Algorithms
 * by Nina Averill <naverill@student.unimelb.edu.au>
 */
#include "list.h"
#include "spell.h"

typedef struct bk_node BKNode;
typedef struct bk_tree BKTree;

/**********************************BK Node***********************************/
struct bk_node {
  char *word;
  List *children;
  int dist_to_parent;
};

BKNode *new_bknode(char *word, int dist);
void free_bknode(BKNode *node);
bool has_children(BKNode *node);

/**********************************BK Tree***********************************/
struct bk_tree {
  BKNode *root;
  int size;
};

BKTree *init_tree();
void free_tree(BKTree *tree);
void free_nodes(BKNode *root);

void insert(BKTree *tree, char *word);
bool sift_down(BKNode *root, BKNode *new);

void search(BKNode *root, char *word, int ldist, char** match);
int d(char *word1, char *word2);
int get_edit_dist(char *word1, char *word2);
int min(int x1, int x2);
void print_array(int *dist, int x, int y);
