#include "parser.h"
#include "redBlack.h"
#include "treeBuilder.h"
#include <string>
#include <iostream>
#include <fstream>

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
	cout << "Parsing " << filename << endl;
	ifstream infile(filename);
	string line = "";
	read_threads = 0;
	write_threads = 0;

	//Parse tree description
	while(getline(infile, line)){
		if(!line.empty()){
			// cout << "Passing to tree builder: " << line << endl;
			tree = tree_builder.string_to_tree(line);
			break;
		}
	}

	//Parse read thread count
	size_t position = 0;
	while(getline(infile, line)){
		position = line.find(":");
		if(position!=string::npos){
			read_threads = stoi(line.substr(position+1));
			break;
		}
	}

	//Parse write thread count
	while(getline(infile, line)){
		// cout << "Finding write threads in line: " << line << endl;
		position = line.find(":");
		if(position!=string::npos){
			write_threads = stoi(line.substr(position+1));
			break;
		}
	}

	while(getline(infile,line)){
		if(line.empty()) continue;
		size_t new_position = 0;
		position = 0;
		while(true){
			new_position = line.find("(", position);
			string action = line.substr(position,new_position-position);
			// cout << "Parsed action: " << action << endl;
			int key = stoi(line.substr(new_position+1));
			// cout << "Parsed key: " << key << endl;
			position = line.find("||", new_position+1);

			JobAction new_job_action;
			if(action == "search"){
				new_job_action = job_search;
			}else if(action == "insert"){
				new_job_action = job_insert;
			}else if(action == "delete"){
				new_job_action = job_remove;
			}
			Job new_job(new_job_action,key);
			jobs.push_back(new_job);

			if(position == string::npos) break;
			position += 3;
		}
	}
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

