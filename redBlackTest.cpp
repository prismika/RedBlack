#include <iostream>
#include "redBlack.h"

using namespace std;

int read_threads = 0;
int write_threads = 0;
/*
vector<Job> jobs;
queue<Job> readers_queue, writers_queue;
*/

RedBlack tree;
/*
Parser parser;
ReportWriter report_writer;
*/


void bad_usage(void){
	cout << "Incorrect usage" << endl;
	return;
}


int main(int argc, char *argv[]){
	if(argc != 2){
		bad_usage();
		return;
	}

	/*
	//TODO Process input string a bit for current directory stuff
	parser.parse(argv[1]);
	read_threads 	= parser.get_num_readers();
	write_threads	= parser.get_num_writers();
	jobs 			= parser.get_jobs();
	tree			= parser.get_tree();
	for(int i = 0; i < read_threads; i++){
		pthread_create(blahblahblah reader_main);
	}
	for(int i = 0; i < write_threads; i++){
		pthread_create(blahblahblah writer_main);
	}

	for(vector<Job>::iterator iter = jobs.begin(); iter != jobs.end(); iter++){
		if(iter->action == search){
			//Queue in reader queue
		}else{
			//Queue in writers queue
		}
	}

	long start_time = time();

	pthreads_set_threads_loose();

	pthreads_wait_for_kids();

	long end_time = time();

	report_writer.output_report(&tree,end_time - start_time);
	*/
	return 0;
}
