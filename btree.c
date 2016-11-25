#include "btree.h"
#include <stdlib.h>
#include <string.h>

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

int btree_split(btree_node_t* split_parent, int ichild, int t)
{
	int i;
	btree_node_t* child = split_parent[ichild];
	btree_node_t* node = btree_alloc_node(t);

	for(i = t; i < 2*t - 1; i++)
	{
		node->key[i-t] = child[i];
		node->nkey++;
		child->nkey--;

		node->childs[i-t] = child->childs[i];
		node->nchild++;
		child->nchild--;
	}

	node->childs[t] = child->child[2*t - 1];
	node->nchild++;
	child->nchild--;

	//shift back util ichild
	for(i = split_parent->nkey; i > ichild; i--)
	{
		split_parent->key[i] = split->key[nkey - 1];
		split_parent->child[i + 1] = split_parent->child[i];
	}

	split_parent->key[ichild - 1] = child->key[t -1];
	split_parent->nkey++;

	split_parent->child[ichild] = node;
	split_parent->nchild++;

	return 0;
}

int btree_insert_not_full(btree_node_type* node, char key, int t)
{
	int i,j;
	
	if(node->leaf)
	{
		i = 0;
		while(i < node->nkey && node->key[i] < key)
			i++;
		
		if(i < node->nkey && node->key[i] == key)
			return 0;
		
		for(j = node->nkey; j < i; j--)
		{
			node->key[j] = node->key[j - 1];
		}
		node->key[i] = key;
	}
	else
	{
		i = 0;
		while(i < node->nkey; && node->key[i] < key)
			i++;
		if(i < node->nkey && node->key[i] == key)
			return 0;

		if(node->child[i]->nkey == 2*t - 1)
		{
			btree_split_node(node,i, t);
			return btree_insert_not_full(node->child[i + 1], key, t);
		}
		else
			return btree_insert_not_full(node->child[i], key, t);
	}


	return 0;
}


int btree_insert(btree_t* tree, char key)
{
	btree_node_t* root = tree->root;
	btree_node_t* node;

	if(root == NULL)
	{
		root = tree->root = btree_alloc_node(tree->t);
	}
	//root is full
	if(root->nkey == 2*tree->t -1)
	{
		node = btree_alloc_node(tree->t);
		tree->root = node;

		root->parent = node;
		node->child[0] = root;
		node->nchild++;

		btree_split_node(node, 0, tree->t);
		btree_insert_not_full(node, key, tree->t);
	}
	else
	{
		btree_insert_not_full(root, key, tree->t);
	}


	return 0;
}

int btree_add(btree_t* tree, char key)
{
	int i;
	btree_node_t* node = tree->root;
	btree_node_t* child;

	if(!node)
	{
		node = btree_alloc_node(tree->t);
		tree->root = node;
	}

	//not full
	if(node->nkey < tree->t)
	{
		i = 0;
		while(i < node->nkey && node->keys[i] < key)
			i++;

		if(node->childs[i] == NULL)
		{
			child = btree_alloc_node(tree->t);
			child->keys[0] = key;
			child->nkey = 1;
			node->childs[i] = child;
			node->nchild++;
		}
		else
		{
			node->keys[i] = key;
			node->nkey++;
		}
	}
	else
	{
	}

	return 0;
}

btree_node_t* btree_alloc_node(int t)
{
	btree_node_t* node = (btree_node_t*)malloc(sizeof(btree_node_t));
	if(node)
	{
		node->keys = (char*)malloc(sizeof(char) * t * 2);
		node->nkey = 0;
		node->childs = (btree_node_t**)malloc(sizeof(btree_node_t*) * t *2);
		memset(node->childs, 0, sizeof(btree_node_t*) * t * 2);
		node->nchild = 0;
		node->leave = 1;
		node->parent = NULL;
	}
	
	return node;
}

btree_node_t* btree_search_node(btree_node_t* node, char key, int* index)
{
	int i = 0;

	while(i < node->nkey && node->keys[i] < key)
		i++;
	
	if(node->keys[i] == key)
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
