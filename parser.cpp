#include "parser.h"
#include "redBlack.h"
#include "treeBuilder.h"
#include <string>
#include <iostream>

using namespace std;

//---------------------------------------
// 				Job
//---------------------------------------

Job::Job(){
	this->action = job_search;
	this->key = 64;
}

JobAction Job::get_action(){
	return this->action;
}

int Job::get_key(){
	return this->key;
}

string Job::to_string(){
	string ret;
	switch(this->action){
		case job_search:
		ret = "search";
		break;
		case job_insert:
		ret = "insert";
		break;
		case job_remove:
		ret = "remove";
		break;
	}
	ret += "(";
	ret += std::to_string(key);
	ret += ")";
	return ret;
}
//---------------------------------------
//				Parser
//---------------------------------------
int Parser::parse(string filename){
	cout << "Parsing " << filename << "..." << endl;
		
	//Parse tree description
	string preorder = "fake";
	tree = tree_builder.string_to_tree(preorder);

	//Parse thread counts
	read_threads = 3;
	write_threads = 2;

	//Parse jobs
	Job *j1 = new Job();
	jobs.push_back(*j1);
	Job *j2 = new Job();
	jobs.push_back(*j2);

	return 0;
}

vector<Job> Parser::get_jobs(){
	return jobs;
}

int Parser::get_num_readers(){
	return read_threads;
}

int Parser::get_num_writers(){
	return write_threads;
}

RedBlack * Parser::get_tree(){
	return tree;
}

