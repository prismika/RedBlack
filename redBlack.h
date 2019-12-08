#ifndef REDBLACK_H
#define REDBLACK_H
#include<pthread.h>

class RedBlackNode{
public:
	int key;
	RedBlackNode *left_child, *right_child, *parent;
	bool red;
};

class RedBlack{
private:
	RedBlackNode *root;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t writer_wait = PTHREAD_COND_INITIALIZER;
	pthread_cond_t reader_wait = PTHREAD_COND_INITIALIZER;
	int writers_writing = 0;
	int readers_reading = 0;
	int readers_waiting = 0;
	RedBlackNode *parent_of(RedBlackNode *node);
	RedBlackNode *sibling_of(RedBlackNode *node);
	void rotate_left(RedBlackNode *node);
	void rotate_right(RedBlackNode *node);
	void before_write();
	void after_write();
	int insert_recurse(RedBlackNode *root, RedBlackNode *node);
	void insert_repair(RedBlackNode *node);
	bool search_recurse(RedBlackNode *root, int key);
	friend class TreeBuilder;
public:
	RedBlack();
	int search(int key);
	int insert(int key);
	int remove(int key);
};

#endif
