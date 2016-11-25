#include "btree.h"
#include <stdlib.h>
#include <string.h>

btree_node_t* btree_alloc_node(int t);

int btree_split_node(btree_node_t* node, int ichild, int t);

int btree_insert_not_full(btree_node_t* node, char key, int t);

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

int btree_split_node(btree_node_t* split_parent, int ichild, int t)
{
	int i;
	btree_node_t* child = split_parent->childs[ichild];
	btree_node_t* node = btree_alloc_node(t);
	node->leaf = child->leaf;
	node->parent = split_parent;

	if(child->leaf)
	{
		for(i = 0; i < t -1 ; i++)
		{
			node->keys[i] = child->keys[i + t];
			child->keys[i+t] = '\0';
			node->nkey++;
			child->nkey--;
		}
	}
	else
	{
		for(i = 0; i < t - 1; i++)
		{
			node->keys[i] = child->keys[i + t];
			child->keys[i+t] = '\0';
			node->nkey++;
			child->nkey--;

			node->childs[i] = child->childs[i+t];
			node->nchild++;
			child->nchild--;
		}

		node->childs[node->nchild++] = child->childs[--child->nchild];

	}

	//shift back util ichild
	for(i = split_parent->nkey; i > ichild; i--)
	{
		split_parent->keys[i] = split_parent->keys[i - 1];
		split_parent->childs[i + 1] = split_parent->childs[i];
	}

	split_parent->keys[ichild] = child->keys[t -1];
	child->keys[t - 1] = '\0';
	split_parent->nkey++;
	child->nkey--;

	split_parent->childs[ichild + 1] = node;
	split_parent->nchild++;

	return 0;
}

int btree_insert_not_full(btree_node_t* node, char key, int t)
{
	int i,j;
	
	if(node->leaf)
	{
		i = 0;
		while(i < node->nkey && node->keys[i] < key)
			i++;
		
		if(i < node->nkey && node->keys[i] == key)
			return 0;
		
		for(j = node->nkey; j < i; j--)
		{
			node->keys[j] = node->keys[j - 1];
		}
		node->keys[i] = key;
		node->nkey++;
	}
	else
	{
		i = 0;
		while(i < node->nkey && node->keys[i] < key)
			i++;
		if(i < node->nkey && node->keys[i] == key)
			return 0;

		if(node->childs[i]->nkey == 2*t - 1)
		{
			btree_split_node(node, i, t);
			return btree_insert_not_full(node->childs[i + 1], key, t);
		}
		else
			return btree_insert_not_full(node->childs[i], key, t);
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
		root->keys[0] = key;
		root->nkey++;
		return 0;
	}
	//root is full
	if(root->nkey == 2*tree->t -1)
	{
		node = btree_alloc_node(tree->t);
		node->leaf = 0;
		tree->root = node;

		root->parent = node;
		node->childs[0] = root;
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
		node->leaf = 1;
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
		int index = i;
		return node;
	}

	if(node->leaf)
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
