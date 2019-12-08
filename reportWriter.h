#ifndef REPORTWRITER_H
#define REPORTWRITER_H

#include <vector>
#include <string>
#include <pthread.h>
#include "parser.h"
#include "redBlack.h"
#include "treeBuilder.h"

using namespace std;

class CompletedJob{
private:
	Job job;
	bool success;
	int thread;
public:
	CompletedJob(Job job, bool success, int thread);
	string to_string();
};

class ReportWriter{
private:
	vector<CompletedJob> completed_jobs;
	long time = 0;
	RedBlack *tree = NULL;
	pthread_mutex_t report_submit_mutex = PTHREAD_MUTEX_INITIALIZER;
	TreeBuilder treeBuilder;
public:
	int report_job(Job job, bool success, int thread);
	int report_time(long time);
	int report_tree(RedBlack * tree);
	int print_report();
};

#endif
