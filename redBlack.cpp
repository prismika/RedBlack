#include "redBlack.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

RedBlack::RedBlack(){
	cout << "Creating a red black tree." << endl;
	pthread_mutex_init(&reader_mutex, NULL);
	pthread_mutex_init(&writer_mutex, NULL);
}

//----------------------------------
//	Reader operations
//----------------------------------
int RedBlack::search(int key){
	pthread_mutex_lock(&reader_mutex);
	cout << "Searching for " << key << endl;
	sleep(0.3);
	cout << "Done searching for " << key << endl;
	pthread_mutex_unlock(&reader_mutex);
	return 0;
}


//----------------------------------
//	Writer operations
//----------------------------------
int RedBlack::insert(int key){
	pthread_mutex_lock(&writer_mutex);
	cout << "Inserting " << key << endl;
	sleep(0.3);
	cout << "Done inserting " << key << endl;
	pthread_mutex_unlock(&writer_mutex);
	return 0;
}

int RedBlack::remove(int key){
	pthread_mutex_lock(&writer_mutex);
	cout << "Removing " << key << endl;
	sleep(0.3);
	cout << "Done removing " << key << endl;
	pthread_mutex_unlock(&writer_mutex);
	return 0;
}
