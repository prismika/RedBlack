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

Job::Job(JobAction action, int key)
	:action(action), key(key)
{}

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
	read_threads = 4;
	write_threads = 4;

	//Parse jobs
	Job j1(job_insert,1);
	jobs.push_back(j1);
	Job j2(job_remove,2);
	jobs.push_back(j2);
	Job j3(job_search,3);
	jobs.push_back(j3);
	Job j4(job_search,4);
	jobs.push_back(j4);
	Job j5(job_insert,5);
	jobs.push_back(j5);
	Job j6(job_insert,6);
	jobs.push_back(j6);
	Job j7(job_remove,7);
	jobs.push_back(j7);
	Job j8(job_search,8);
	jobs.push_back(j8);
	Job j9(job_search,9);
	jobs.push_back(j9);

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

