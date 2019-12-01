#ifndef TREEBUILDER_H
#define TREEBUILDER_H

#include <string>
#include "redBlack.h"

using namespace std;

class TreeBuilder{
public:
	RedBlack *string_to_tree(string str);
	string tree_to_string(RedBlack *tree);
};

#endif
