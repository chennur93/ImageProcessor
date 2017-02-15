#ifndef SRC_THREADS_HPP_
#define SRC_THREADS_HPP_

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "queue.hpp"

class threads {
private:
	pthread_t pid;
	int data;
	//THREADSTATUS status;
	pthread_attr_t attr;

public:

	threads();

	~threads();

	pthread_t *getPid();

	int getData();

	void setData(int socketfd);

	void setPriorityMax();

	void setPriorityMin();

	pthread_attr_t *getAttr();
};


#endif /* SRC_THREADS_HPP_ */
