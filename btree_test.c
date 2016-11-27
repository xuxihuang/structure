#include "btree.h"

int main(int argc, char** argv)
{
	btree_t* tree;
	btree_node_t* node;
	int index;

	tree = btree_init(2);

	btree_insert(tree, 'A');

	btree_insert(tree, 'B');

	btree_insert(tree, 'C');

	btree_insert(tree, 'D');

	btree_insert(tree, 'E');

	btree_insert(tree, 'F');

	btree_insert(tree, 'G');

	node = btree_search(tree, 'I', &index);

	node = btree_search(tree, 'G', &index);

	node = btree_search(tree, 'C', &index);

	return 0;
}
