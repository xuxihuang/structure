/*This is a practice of btree implement by xuxi huang.The programm never test
 * or use in any commercial mechine.You should be careful if you use the code.
 * */
#ifndef __BTREE_H__
#define __BTREE_H__

typedef struct btree_node_s
{
	char*	keys;
	int	nkey;
	struct btree_node_s**	childs;
	int	nchild;
	int	leaf;
	struct btree_node_s*	parent;
} btree_node_t;

typedef struct btree_s
{
	int	t;
	btree_node_t* root;
} btree_t;

btree_t* btree_init(int t);

int btree_add(btree_t* tree, char key);

btree_node_t*	btree_search(btree_t* tree, char key, int* index);

int btree_delete(btree_t* tree, char key);

#endif

