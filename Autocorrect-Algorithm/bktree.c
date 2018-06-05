/* * * * * * *
 * Module for creating, manipulating and searching BK Trees containing strings
 *
 * created for COMP20007 Design of Algorithms
 * by Nina Averill <naverill@student.unimelb.edu.au>
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "bktree.h"
#include "list.h"
#include "spell.h"

#define TOLERANCE 3

/**********************************BK Node***********************************/
/**
* CREATE A NEW BK TREE NODE
*/
BKNode *new_bknode(char *word, int dist){
  assert(word);

  BKNode *new = malloc(sizeof(BKNode));
  assert(new);

  new->word = word;
  new->children = NULL;
  new->dist_to_parent = dist;

  return new;
}

/**
* FREE BK TREE NODE
*/
void free_bknode(BKNode *node){
  assert(node);
  if(node->children){
    free_list(node->children);
  }
  free(node);
}

/**
* RETURN BOOLEAN VALUE INDICATING IF BK TREE NODE HAS CHILD NODES
*/
bool has_children(BKNode *node){
  return node->children && list_size(node->children)>0;
}
/**********************************BK Tree***********************************/
/**
* CREATE EMPTY BK TREE
*/
BKTree *init_tree(){
  BKTree *tree = malloc(sizeof(BKTree));
  assert(tree);

  tree->root = NULL;
  tree->size = 0;

  return tree;
}

/**
* FREE BK TREE
*/
void free_tree(BKTree *tree){
  assert(tree);
  if(tree->root){
    free_nodes(tree->root);
  }
  free(tree);
}

/**
* FREE BK TREE NODE `root` AND ALL CHILD NODES
*/
void free_nodes(BKNode *root){
  assert(root);
  if(has_children(root)){
    Node *child = root->children->head;

    while(child){
      free_nodes(child->data);
      child = child->next;
    }
  }
  free_bknode(root);
  return;
}

/**
* INSERT WORD IN BK TREE
*/
void insert(BKTree *tree, char *word){
  assert(word);
  assert(tree);

  BKNode *node = new_bknode(word, -1);

  if(!tree->root){
    tree->root = node;
    tree->size += 1;
    return;
  }

  //shift node down tree until appropriately located
  if(sift_down(tree->root, node)){
    tree->size += 1;
  }
}

/**
* SHIFT NEW WORD DOWN BK TREE
*/
bool sift_down(BKNode *root, BKNode *new){
  if(!new || !root){
    return false;
  }

  int dist = d(root->word, new->word);

  //word already in tree
  if(dist == 0){
    return false;
  }

  if(!root->children){
    root->children = new_list();
  }

  //iterate through children
  Node *child = root->children->head;
  while(child){
    //if a distance collision occurs, make `new` a child of the
    //current child node
    if(((BKNode *)child->data)->dist_to_parent == dist){
      return sift_down(child->data, new);

    } else {
      child = child->next;
    }
  }

  //if distance not found, add `new` as child of current node
  new->dist_to_parent = dist;
  list_add_end(root->children, new);
  return true;
}

/**
* SEARCH BK TREE FOR CLOSEST MATCH TO `word` WITH LEVENSHTEIN DIST <= `ldist`
*/
void search(BKNode *root, char *word, int ldist, char **match){
  assert(root);
  assert(word);
  //exit if a match is found
  if(*match!=NULL){
    return;
  }

  int pdist = d(root->word, word);

  //closest match found
  if(pdist <= ldist){
    *match = root->word;
    return;
  }

  //Match is not located in tree
  if(!has_children(root)){
    return;
  }

  //search through children until match is found with Levenshtein
  //distance <= ldist
  Node *child = root->children->head;
  while(child && *match==NULL){
    if((BKNode *)child->data){
      int cdist = ((BKNode *)child->data)->dist_to_parent;
      //search child if dist is in range [pdist - ldist, pdist + ldist]
      if(pdist - ldist <= cdist && cdist <= pdist + ldist){
        search(child->data, word, ldist, match);
      }
    }
    child=child->next;
  }
  return;
}

/******************************Helper Functions*******************************/
/**
* FIND DISTANCE BETWEEN NODES
*/
int d(char *word1, char *word2){
  return get_edit_dist(word1, word2);
}

/**
* CALCULATE EDIT DISTANCE
*/
int get_edit_dist(char *word1, char *word2){
	int s1 = strlen(word1);
	int s2 = strlen(word2);

	int dist[s1+1][s2+1];

	int i, j;
	for(i=0; i<=s1; i++){
		dist[i][0] = i;
	}

	for(j=1; j<=s2; j++){
		dist[0][j] = j;
	}

	int cost, insertion, deletion, substitution;
	for(i=1; i<=s1; i++){
		for(j=1; j<=s2; j++){

      //if characters are not equal, substitute letter with cost of 1
      // else cost is 0
			cost = (word1[i-1] == word2[j-1]) ? 0 : 1;

			substitution = dist[i-1][j-1] + cost;
			deletion = dist[i-1][j] + 1;
			insertion = dist[i][j-1] + 1;

      //get minimum cost of all three edit operations
			dist[i][j] = min(min(substitution, insertion), deletion);
		}
	}
	return dist[s1][s2];
}

/**
* RETURN MINIMUM OF TWO INTEGERS
*/
int min(int x1, int x2){
	return (x1 < x2) ? x1 : x2;
}

/**
* PRINT 2D Array
*/
void print_array(int *dist, int x, int y){
	int i, j;
	for(i=0; i<x; i++){
		for(j=0; j<y; j++){
			printf("%d ", *((dist + i*y) + j));
		}
		printf("\n");
	}
	printf("\n");
}
