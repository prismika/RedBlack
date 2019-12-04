#ifndef REDBLACK_H
#define REDBLACK_H
#include<pthread.h>

class RedBlackNode{
private:
	int key;
	RedBlackNode *left_child, *right_child;
	bool red;
};

class RedBlack{
private:
	RedBlackNode root;
	pthread_mutex_t reader_mutex;
	pthread_mutex_t writer_mutex;
public:
	RedBlack();
	int search(int key);
	int insert(int key);
	int remove(int key);
};





#endif
