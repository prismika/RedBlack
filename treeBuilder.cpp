#include "treeBuilder.h"
#include <string>
#include <queue>
#include <iostream>
#include "redBlack.h"


using namespace std;

void TreeBuilder::construct_tree_recurse(RedBlackNode *root, queue<RedBlackNode*> *node_queue){
	if(root == NULL){
		return;
	}
	//Construct left subtree
	if(node_queue->empty()){
		cout << "Node queue was unexpectedly empty." << endl;
		return;
	}
	RedBlackNode * left_node = node_queue->front();
	node_queue->pop();

	if(left_node){
		left_node->parent = root;
	}
	root->left_child = left_node;
	construct_tree_recurse(left_node, node_queue);

	//Construct right subtree
	if(node_queue->empty()){
		cout << "Node queue was unexpectedly empty." << endl;
		return;
	}
	RedBlackNode * right_node = node_queue->front();
	node_queue->pop();

	if(right_node){
		right_node->parent = root;
	}
	root->right_child = right_node;
	construct_tree_recurse(right_node, node_queue);
}

RedBlack *TreeBuilder::string_to_tree(string str){
	RedBlack *tree = new RedBlack();
	size_t str_position = 0;
	size_t new_str_position = 0;
	queue<RedBlackNode*> node_queue;

	//Fill the queue with nodes
	while(new_str_position != string::npos){
		//Parse the next token
		new_str_position = str.find(",", str_position);
		string token = str.substr(str_position, new_str_position - str_position);
		str_position = new_str_position+1;

		// cout << "Got token " << token << endl;

		// Create a new node from the token
		RedBlackNode *new_node;
		if(token.find("f") == string::npos){
			new_node = new RedBlackNode();
			new_node -> key = stoi(token);
			new_node -> left_child = NULL;
			new_node -> right_child = NULL;
			new_node -> red = (token.find("r") != string::npos);
			// cout << "Constructed node with key " << new_node -> key
			// 	<< ", color " << (new_node->red ? "red" : "black")
			// 	<< endl;
		}else{
			new_node = NULL;
			// cout << "Constructed null node." << endl;
		}
		node_queue.push(new_node);
	}

	tree -> root = node_queue.front();
	node_queue.pop();
	construct_tree_recurse(tree->root,&node_queue);

	return tree;
}

void TreeBuilder::tree_to_string_recurse(string* str, RedBlackNode* root){
	if(!root){
		str->append("f,");
		return;
	}

	string to_append = "";
	to_append += std::to_string(root->key);
	to_append += root->red ? "r" : "b";
	to_append += ",";
	str->append(to_append);
	tree_to_string_recurse(str, root->left_child);
	tree_to_string_recurse(str, root->right_child);
}


string TreeBuilder::tree_to_string(RedBlack *tree){
	string ret = "";
	tree_to_string_recurse(&ret,tree->root);
	//Clip trailing comma
	ret = ret.substr(0,ret.size()-1);
	return ret;
}

