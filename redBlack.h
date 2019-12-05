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
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t writer_wait = PTHREAD_COND_INITIALIZER;
	pthread_cond_t reader_wait = PTHREAD_COND_INITIALIZER;
	int writers_writing = 0;
	int readers_reading = 0;
	int readers_waiting = 0;
	void before_write();
	void after_write();
public:
	RedBlack();
	int search(int key);
	int insert(int key);
	int remove(int key);
};

#endif
