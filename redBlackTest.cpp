#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include "redBlack.h"
#include "parser.h"
#include "reportWriter.h"

using namespace std;

int num_read_threads = 0;
int num_write_threads = 0;
vector<Job> jobs;
queue<Job> readers_queue, writers_queue;
pthread_mutex_t readers_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writers_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

RedBlack *tree;
Parser parser;
ReportWriter report_writer;

pthread_barrier_t on_your_marks, finish_line;


void bad_usage(void){
	cout << "Incorrect usage" << endl;
	return;
}

int run_job(Job job){
	switch(job.get_action()){
		case job_search:
		return tree->search(job.get_key());
		case job_insert:
		return tree->insert(job.get_key());
		case job_remove:
		return tree->remove(job.get_key());
	}
	return -2;
}

void* reader_main(void* arg){
	// cout << "Reader created" << endl;
	pthread_barrier_wait(&on_your_marks);
	while(true){
		//Aquire the mutex
		pthread_mutex_lock(&readers_queue_mutex);
		//Check if there are more jobs
		if(readers_queue.empty()){
			pthread_mutex_unlock(&readers_queue_mutex);
			break;
		}
		//Get the next job
		Job current_job = readers_queue.front();
		readers_queue.pop();
		//Release the mutex
		pthread_mutex_unlock(&readers_queue_mutex);

		//Run the job
		int status = run_job(current_job);
		//Submit the report
		report_writer.report_job(current_job, (status == 0), pthread_self());
	}
	pthread_barrier_wait(&finish_line);
	return NULL;
}

void* writer_main(void* arg){
	// cout << "Writer created" << endl;
	pthread_barrier_wait(&on_your_marks);
	while(true){

		pthread_mutex_lock(&writers_queue_mutex);
		if(writers_queue.empty()){
			pthread_mutex_unlock(&writers_queue_mutex);
			break;
		}
		Job current_job = writers_queue.front();
		writers_queue.pop();
		pthread_mutex_unlock(&writers_queue_mutex);

		int status = run_job(current_job);
		report_writer.report_job(current_job, (status == 0), pthread_self());
	}
	pthread_barrier_wait(&finish_line);
	return NULL;
}



int main(int argc, char *argv[]){
	if(argc != 2){
		bad_usage();
		return -1;
	}
	
	//TODO Process input string a bit for current directory stuff
	//Tell parser to parse the specified file
	string filename = getenv("PWD");
	filename += "/";
	filename += argv[1];
	parser.parse(filename);
	num_read_threads 	= parser.get_num_readers();
	num_write_threads	= parser.get_num_writers();
	jobs 			= parser.get_jobs();
	tree			= parser.get_tree();

	//Count threads and create barriers
	int num_threads_total = num_read_threads+num_write_threads+1;
	cout << "Read threads: " << num_read_threads << endl;
	cout << "Write threads: " << num_write_threads << endl;
	cout << "Total threads: " << num_threads_total << endl;
	pthread_barrier_init(&on_your_marks,NULL,num_threads_total);
	pthread_barrier_init(&finish_line,NULL,num_threads_total);
	
	//Create threads
	pthread_t *read_threads = (pthread_t*) malloc(num_read_threads * sizeof(pthread_t));
	pthread_t *write_threads = (pthread_t*) malloc(num_write_threads * sizeof(pthread_t));
	for(int i = 0; i < num_write_threads; i++){
		// cout << "Creating writer " << i+1 << endl;
		pthread_create(&write_threads[i], NULL, writer_main, NULL);

	}
	for(int i = 0; i < num_read_threads; i++){
		// cout << "Creating reader " <<  i+1 << endl;
		pthread_create(&read_threads[i], NULL, reader_main, NULL);
	}

	//Sort jobs into necessary queues
	for(vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); iter++){
		if(iter->get_action() == job_search){
			// cout << "Placing job: " << iter->to_string() << " in reader queue." << endl;
			readers_queue.push(*iter);
		}else{
			// cout << "Placing job: " << iter->to_string() << " in writer queue." << endl;
			writers_queue.push(*iter);
		}
	}

	// long start_time = time();

	cout << "Preparing to release all threads..." << endl;
	pthread_barrier_wait(&on_your_marks);

	cout << "Waiting on all threads to finish" << endl;
	pthread_barrier_wait(&finish_line);
	// long end_time = time();

	// report_writer.report_time(end_time - start_time);

	report_writer.report_tree(tree);

	report_writer.print_report();
	
	return 0;
}
