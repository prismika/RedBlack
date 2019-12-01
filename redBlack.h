#ifndef REDBLACK_H
#define REDBLACK_H


class RedBlackNode{
private:
	int key;
	RedBlackNode *left_child, *right_child;
	bool red;
};

class RedBlack{
private:
	RedBlackNode root;
public:
	RedBlack();
	int search(int key);
	int insert(int key);
	int remove(int key);
};





#endif
