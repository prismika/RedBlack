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
	}else if(root && node->key > root->key){
		//If root has a right child
		if(root->right_child){
			//recurse on that child
			return insert_recurse(root->right_child,node);
		}else{
			root->right_child = node;
		}
	}else if(root){
		//Root's key matches node's key. It's a setup. Get outta there.
		return -1;
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
	}else if(key < root->key){
		return search_recurse(root->left_child, key);
	}else{
		return search_recurse(root->right_child, key);
	}
}

RedBlackNode *RedBlack::remove_recurse(RedBlackNode *root, int key){
	if(!root){
		return NULL;
	}else if(root->key == key){
		return root;
	}else if(key < root->key){
		return remove_recurse(root->left_child, key);
	}else{
		return remove_recurse(root->right_child, key);
	}
}

//Put child where node currently is in the tree
void RedBlack::replace_node(RedBlackNode *node, RedBlackNode *child){
	child->parent = node->parent;
	//If node is a left child
	if(node == node->parent->left_child){
		node->parent->left_child = child;
	//If node is right child
	}else{
		node->parent->right_child = child;
	}
}

void RedBlack::remove_node_with_one_child(RedBlackNode *node){
	RedBlackNode* chosen_child;
	if(node->right_child == NULL){
		chosen_child = node->left_child;
	}else{
		chosen_child = node->right_child;
	}

	replace_node(node, chosen_child);
	if(!node->red){
		//If node is black and child is red
		if(chosen_child->red){
			//recolor it to black
			chosen_child->red = false;
		}else{
			remove_case_1(chosen_child);
		}
	}
}

void RedBlack::remove_case_1(RedBlackNode *node){
	//If child is now the root, we are done
	if(!node->parent){
		return;
	}
	remove_case_2(node);
}

void RedBlack::remove_case_2(RedBlackNode *node){
	RedBlackNode *sibling = sibling_of(node);
	if(sibling->red){
		node->parent->red = true;
		sibling->red = false;
		if(node == node->parent->left_child){
			rotate_left(node->parent);
		}else{
			rotate_right(node->parent);
		}
	}
	remove_case_3(node);
}

void RedBlack::remove_case_3(RedBlackNode *node){
	RedBlackNode *sibling = sibling_of(node);
	//If parent, sibling, and both nephews are black
	if(!node->parent->red
		&& !sibling->red
		&& !sibling->left_child->red
		&& !sibling->right_child->red){
		sibling->red = true;
		remove_case_1(node->parent);
	}else{
		remove_case_4(node);
	}
}

void RedBlack::remove_case_4(RedBlackNode *node){
	RedBlackNode *sibling = sibling_of(node);
	//If parent is red, and sibling and its kids are black
	if(node->parent->red
		&& !sibling->red
		&& !sibling->left_child->red
		&& !sibling->right_child->red){
		sibling->red = true;
		node->parent->red = false;
	}else{
		remove_case_5(node);
	}
}

void RedBlack::remove_case_5(RedBlackNode *node){
	RedBlackNode *sibling = sibling_of(node);
	if(!sibling->red){
		//If node is a left child,
		//and the sibling's left child is the only red one
		if((node == node->parent->left_child)
			&& !(sibling->right_child->red)
			&& (sibling->left_child->red)){
			sibling->red = true;
			sibling->left_child->red = false;
			rotate_right(sibling);
		}else if((node == node->parent->right_child)
			&& !(sibling->left_child->red)
			&& (sibling->right_child->red)){
			sibling->red = true;
			sibling->right_child->red = false;
			rotate_left(sibling);
		}
	}
	remove_case_6(node);
}


void RedBlack::remove_case_6(RedBlackNode *node){
	RedBlackNode *sibling = sibling_of(node);
	sibling->red = node->parent->red;
	node->parent->red = false;

	if(node == node->parent->left_child){
		sibling->right_child->red = false;
		rotate_left(node->parent);
	}else{
		sibling->left_child->red = false;
		rotate_right(node->parent);
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

	RedBlackNode *node = remove_recurse(this->root,key);
	int remove_status = (node == NULL)? 1 : 0;
	if(remove_status == 0){
		//If key was found...
		//If node has left child,
		if(node->left_child){
			RedBlackNode *lefty = node->left_child;
			while(lefty->right_child){
				lefty = lefty->right_child;
			}
			//Lefty is now the next largest element of the tree
			node->key = lefty->key;
			//Lefty must have one child
			remove_node_with_one_child(lefty);
		//If node has a right child,
		}else if(node->right_child){
			RedBlackNode *righty = node->right_child;
			while(righty->left_child){
				righty = righty->left_child;
			}
			//Righty is now the next smallest element of the tree
			node->key = righty->key;
			remove_node_with_one_child(righty);
		}
	}

	after_write();
	return remove_status;
}
