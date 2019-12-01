#ifndef PARSER_H
#define PARSER_H

#include "redBlack.h"
#include <vector>
#include <string>

using namespace std;

typedef enum {
	job_search,
	job_insert,
	job_remove
}JobAction;

class Job{
private:
	JobAction action;
	int key;
public:
	Job();
	JobAction get_action();
	int get_key();
	string to_string();
};

class Parser{
private:
	RedBlack *tree;
	int read_threads = 0;
	int write_threads = 0;
	vector<Job> jobs;
public:
	int parse(string filename);
	vector<Job> get_jobs();
	int get_num_readers();
	int get_num_writers();
	RedBlack *get_tree();
};

#endif
