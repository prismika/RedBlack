#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include <string>
#include <queue>
#include "redBlack.h"

using namespace std;

class TreeBuilder{
private:
	void construct_tree_recurse(RedBlackNode *root, queue<RedBlackNode*> *node_queue);
	void tree_to_string_recurse(string* str, RedBlackNode* root);
public:
	RedBlack *string_to_tree(string str);
	string tree_to_string(RedBlack *tree);
};

#endif
