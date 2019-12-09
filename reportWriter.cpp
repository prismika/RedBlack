#include "reportWriter.h"

#include <vector>
#include <string>
#include <iostream>
#include <pthread.h>
#include "parser.h"
#include "treeBuilder.h"


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

	pthread_mutex_lock(&report_submit_mutex);
	completed_jobs.push_back(new_job);
	// cout << "Job reported: " << new_job.to_string() << endl;
	pthread_mutex_unlock(&report_submit_mutex);
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
	cout << "\nResults:" << endl;

	cout << treeBuilder.tree_to_string(this->tree) << endl;

	cout << "Elapsed time: " << this->time << endl;

	for(vector<CompletedJob>::iterator iter = completed_jobs.begin(); iter != completed_jobs.end(); iter++){
		cout << iter->to_string() << endl;
	}
	return 0;
}
