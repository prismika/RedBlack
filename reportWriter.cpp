#include "reportWriter.h"

#include <vector>
#include <string>
#include <iostream>
#include "parser.h"

using namespace std;


CompletedJob::CompletedJob(Job job, bool success, int thread){
	this->job = job;
	this->success = success;
	this->thread = thread;
	return;
}

string CompletedJob::to_string(){
	string ret = "";
	ret += job.to_string();
	ret += " -> ";
	ret += success ? "true" : "false";
	ret += ", performed by thread: ";
	ret += std::to_string(thread);
	return ret;
}

int ReportWriter::report_job(Job job, bool success, int thread){
	CompletedJob new_job(job,success,thread);
	completed_jobs.push_back(new_job);
	return 0;
}

int ReportWriter::report_time(long time){
	this->time = time;
	return 0;
}

int ReportWriter::report_tree(RedBlack * tree){
	this->tree = tree;
	return 0;
}

int ReportWriter::print_report(){
	cout << "Results:\n" << endl;

	for(vector<CompletedJob>::iterator iter = completed_jobs.begin(); iter != completed_jobs.end(); iter++){
		cout << iter->to_string() << endl;
	}
	return 0;
}
