#include "parser.h"
#include "redBlack.h"
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
	return 0;
}

vector<Job> Parser::get_jobs(){
	vector<Job> ret;
	Job j1;
	ret.push_back(j1);
	Job j2;
	ret.push_back(j2);
	return ret;
}

int Parser::get_num_readers(){
	return 3;
}

int Parser::get_num_writers(){
	return 2;
}

RedBlack * Parser::get_tree(){
	RedBlack * tree = new RedBlack();
	return tree;
}

