#include "btree.h"
#include <stdlib.h>

btree_node_t* btree_alloc_node(int t);

btree_t* btree_init(int t)
{
	btree_t* tree = (btree_t*)malloc(sizeof(btree_t));
	if(tree)
	{
		tree->t = t;
		tree->root = NULL;
	}

	return tree;
}

int btree_add(btree_t* tree, char key)
{
	btree_node_t* node = tree->root;
	
	if(!node)
	{
		node = btree_alloc_node(tree->t);
		tree->root = node;
	}



	return 0;
}

btree_node_t* btree_alloc_node(int t)
{
	btree_node_t* node = (btree_node_t*)malloc(sizeof(btree_node_t));
	if(node)
	{
		node->keys = (char**)malloc(sizeof(char*) * t * 2);
		nkey = 0;
		node->childs = (btree_node_t**)(sizeof(btree_node_t*) * t *2);
		node->nchild = 0;
		node->leave = 1;
		node->parent = NULL;
	}
	
	return node;
}

btree_node_t* btree_search_node(btree_node_t* node, char key, int* index)
{
	int i = 0;

	while(i < node->nkeys && *(node->keys[i]) < key)
		i++;
	
	if(*(node->keys[i]) == key)
	{
		index = i;
		return node;
	}

	if(node->leave)
		return NULL;

	return btree_search_node(node->childs[i], key, index);
}

btree_node_t* btree_search(btree_t* tree, char key, int* index)
{
	btree_node_t* node = tree->root;
	int i;

	if(!node)
		return NULL;

	return btree_search_node(node, key, index);	
}
