#include "redBlack.h"
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

RedBlack::RedBlack(){
	cout << "Creating a red black tree." << endl;
}

//----------------------------------
//	Reader operations
//----------------------------------
int RedBlack::search(int key){
	pthread_mutex_lock(&mutex);
	//If someone is writing
	if(writers_writing>0){
		readers_waiting++;
		pthread_cond_wait(&reader_wait, &mutex);
	}
	readers_reading++;
	pthread_mutex_unlock(&mutex);

	// cout << "Search" << endl;
	sleep(1);

	pthread_mutex_lock(&mutex);
	readers_reading--;
	if(readers_reading == 0){
		pthread_cond_signal(&writer_wait);
	}
	pthread_mutex_unlock(&mutex);
	return 0;
}

//----------------------------------
//	Writer operations
//----------------------------------
void RedBlack::before_write(){
	pthread_mutex_lock(&mutex);
	//If anyone is reading or writing, or readers are waiting,
	if(readers_reading>0 || writers_writing>0 || readers_waiting>0){
		//wait to be called
		pthread_cond_wait(&writer_wait, &mutex);
	}
	writers_writing++;
	pthread_mutex_unlock(&mutex);
	return;
}

void RedBlack::after_write(){
	pthread_mutex_lock(&mutex);
	writers_writing--;
	//If readers are waiting,
	if(readers_waiting>0){
		//release them.
		readers_waiting = 0;
		pthread_cond_broadcast(&reader_wait);
	}else{
		pthread_cond_signal(&writer_wait);
	}
	pthread_mutex_unlock(&mutex);
}

int RedBlack::insert(int key){
	before_write();

	// cout << "M: Insert" << endl;
	sleep(1);

	after_write();
	return 0;
}

int RedBlack::remove(int key){
	before_write();

	// cout << "M: Remove" << endl;
	sleep(1);
	
	after_write();
	return 0;
}
