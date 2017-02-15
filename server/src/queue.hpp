#ifndef SRC_QUEUE_HPP_
#define SRC_QUEUE_HPP_
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

using namespace std;

class Queue {
private:
	const int32_t size;
	int32_t head;
	int32_t tail;
	int *buffer;

public:
	pthread_mutex_t rmutex;
	pthread_cond_t rcond;
	pthread_cond_t wcond;

	Queue(int32_t size);

	~Queue();

	void resetQueue();

	void enqueue(int data);

	int dequeue();

	bool isQueueFull();

};

class QueueFull : public exception{};
class QueueEmpty : public exception{};


#endif /* SRC_QUEUE_HPP_ */
