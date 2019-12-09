#include "redBlack.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

RedBlack::RedBlack(){
	root = NULL;
}

//----------------------------------
//	Private functions
//----------------------------------

RedBlackNode *RedBlack::parent_of(RedBlackNode* node){
	if(node == NULL){
		return NULL;
	}else{
		return node->parent;
	}
}

RedBlackNode *RedBlack::sibling_of(RedBlackNode* node){
	RedBlackNode *parent = parent_of(node);
	if(parent == NULL) return NULL;
	//If node is a left_child
	if(node == parent->left_child){
		//return the right_child
		return parent->right_child;
	}else{
		//Otherwise return the left_child
		return parent->left_child;
	}
}

void RedBlack::rotate_left(RedBlackNode *node){
	RedBlackNode *parent = parent_of(node);
	RedBlackNode *replacement = node->right_child;
	
	//Replace appropriate pointers
	node->right_child = replacement->left_child;
	replacement->left_child = node;
	node->parent = replacement;
	if(node->right_child){
		node->right_child->parent = node;
	}
	if(parent){
		if(node == parent->left_child){
			parent->left_child = replacement;
		}else if (node == parent->right_child){
			parent->right_child = replacement;
		}
	}
	replacement->parent = parent;
}

void RedBlack::rotate_right(RedBlackNode *node){
	RedBlackNode *parent = parent_of(node);
	RedBlackNode *replacement = node->left_child;
	
	//Replace appropriate pointers
	node->left_child = replacement->right_child;
	replacement->right_child = node;
	node->parent = replacement;
	if(node->left_child){
		node->left_child->parent = node;
	}
	if(parent){
		if(node == parent->left_child){
			parent->left_child = replacement;
		}else if(node == parent->right_child){
			parent->right_child = replacement;
		}
	}
	replacement->parent = parent;
}

int RedBlack::insert_recurse(RedBlackNode *root, RedBlackNode *node){
	if(node->key == root->key) return -1;
	//If root is not a leaf and root's key is too large
	if(root && node->key < root->key){
		//If root has a left child
		if(root->left_child){
			//recurse on that child
			return insert_recurse(root->left_child,node);
		}else{//if not,
			//it has one now
			root->left_child = node;
		}
	//If root is not a leaf but it's key is not too large,
	}else if(root){
		//If root has a right child
		if(root->right_child){
			//recurse on that child
			return insert_recurse(root->right_child,node);
		}else{
			root->right_child = node;
		}
	}
	//Node is attached. Set its pointers
	node->parent = root;
	node->left_child = NULL;
	node->right_child = NULL;
	node->red = true; //Red for now
	return 0;
}

void RedBlack::insert_repair(RedBlackNode *node){
	//If node is root
	if(parent_of(node)==NULL){					//Case 1
		node->red = false; //Root is black
	//If node has black parent
	}else if(!parent_of(node)->red){			//Case 2
		//All is well
	//If node has a red uncle
	}else if(  sibling_of(parent_of(node))
			&& sibling_of(parent_of(node))->red){//Case 3
		//color parent and uncle black
		parent_of(node)->red = false;
		sibling_of(parent_of(node))->red = false;
		//Color grandparent red
		parent_of(parent_of(node))->red = true;
		//Repair the grandparent
		insert_repair(parent_of(parent_of(node)));
	}else{//Node has red parent, no red uncle	//Case 4
		//Things get a bit complicated
		RedBlackNode* parent = parent_of(node);
		RedBlackNode* grandparent = parent_of(parent);
		//If node is "under" grandparent
		if(node == parent->right_child
			&& parent == grandparent->left_child){
			rotate_left(parent);
			node = node->left_child;
		}else if(node == parent->left_child
			&& parent == grandparent->right_child){
			rotate_right(parent);
			node = node->right_child;
		}
		//Now we know node is not "under" its grandparent
		//We might have reordered the tree, so set these again
		parent = parent_of(node);
		grandparent = parent_of(parent);
		if(node == parent->left_child){
			rotate_right(grandparent);
		}else{
			rotate_left(grandparent);
		}
		parent->red = false;
		grandparent->red = true;
	}
}

bool RedBlack::search_recurse(RedBlackNode *root, int key){
	if(!root){
		return false;
	}
	if(root->key == key){
		return true;
	}else{
		return search_recurse(root->left_child, key)
		|| search_recurse(root->right_child, key);
	}
}

//----------------------------------
//	Reader operations
//----------------------------------
int RedBlack::search(int key){
	pthread_mutex_lock(&mutex);
	//If someone is writing
	if(writers_writing>0){
		readers_waiting++;
		pthread_cond_wait(&reader_wait, &mutex);
	}
	readers_reading++;
	pthread_mutex_unlock(&mutex);
	//------End of multithreading stuff-------
	
	bool result = search_recurse(this->root, key);

	//------End of multithreading stuff-------
	pthread_mutex_lock(&mutex);
	readers_reading--;
	if(readers_reading == 0){
		pthread_cond_signal(&writer_wait);
	}
	pthread_mutex_unlock(&mutex);
	return result ? 0 : 1;
}

//----------------------------------
//	Writer operations
//----------------------------------
void RedBlack::before_write(){
	pthread_mutex_lock(&mutex);
	//If anyone is reading or writing, or readers are waiting,
	if(readers_reading>0 || writers_writing>0 || readers_waiting>0){
		//wait to be called
		pthread_cond_wait(&writer_wait, &mutex);
	}
	writers_writing++;
	pthread_mutex_unlock(&mutex);
	return;
}

void RedBlack::after_write(){
	pthread_mutex_lock(&mutex);
	writers_writing--;
	//If readers are waiting,
	if(readers_waiting>0){
		//release them.
		readers_waiting = 0;
		pthread_cond_broadcast(&reader_wait);
	}else{
		pthread_cond_signal(&writer_wait);
	}
	pthread_mutex_unlock(&mutex);
}

int RedBlack::insert(int key){
	before_write();

	RedBlackNode *new_node = new RedBlackNode();
	new_node->key = key;
	new_node->left_child = NULL;
	new_node->right_child = NULL;
	new_node->parent = NULL;
	new_node->red = true;

	int insert_status = insert_recurse(this->root, new_node);
	if(insert_status == 0){
		//Node did not already exist. Tree was edited
		insert_repair(new_node);
		//The root might have changed which would be a disaster
		RedBlackNode *cur_node = new_node;
		while(parent_of(cur_node)){
			cur_node = parent_of(cur_node);
		}
		this->root = cur_node;
	}

	after_write();
	return insert_status;
}

int RedBlack::remove(int key){
	before_write();

	// cout << "M: Remove" << endl;
	sleep(1);
	
	after_write();
	return 0;
}
