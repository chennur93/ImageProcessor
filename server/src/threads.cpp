#include "threads.hpp"

using namespace std;

threads::threads() {
	pid = 0;
	data = 0;
	//status = EMPTY;
	pthread_attr_init(&attr);
}

threads::~threads() {
	pthread_attr_destroy(&attr);
}

pthread_t *threads::getPid() {
	return &pid;
}

void threads::setData(int socketfd) {
	this->data = socketfd;
}

int threads::getData() {
	return data;
}

pthread_attr_t *threads::getAttr() {
	return &attr;
}

void threads::setPriorityMax() {
	sched_param priority;
	priority.__sched_priority = sched_get_priority_max(SCHED_RR);
	pthread_setschedparam(pid, SCHED_RR, &priority);
}

void threads::setPriorityMin() {
	sched_param priority;
	priority.__sched_priority = sched_get_priority_min(SCHED_RR);
	pthread_setschedparam(pid, SCHED_RR, &priority);
}
