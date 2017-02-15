#include "queue.hpp"

Queue::Queue(int32_t size) : size(size){
	//this->size = size;
	this->head = 0;
	this->tail = -1;
	this->buffer = new int32_t[size];
	pthread_mutex_init(&rmutex,NULL);
	pthread_cond_init(&rcond,NULL);
	pthread_cond_init(&wcond,NULL);
}

Queue::~Queue() {
	delete this->buffer;
	pthread_mutex_destroy(&rmutex);
	pthread_cond_destroy(&rcond);
	pthread_cond_destroy(&wcond);
}

void Queue::resetQueue() {
	head = 0;
	tail = -1;
}

void Queue::enqueue(int data) {
	if (tail + head == size - 1) {
		throw QueueFull();
	}
	tail %= size;
	this->buffer[++tail] = data;
}

int Queue::dequeue() {
	if (head == tail + 1) {
		throw QueueEmpty();
	}
	head %= size;
	return buffer[head++];
}

bool Queue::isQueueFull() {
	if (tail + head == size - 1) {
		return true;
	}
	return false;
}


