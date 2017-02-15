#include "imageManagement.hpp"
#include "sockethandler.hpp"
#include "threads.hpp"

#define MAX_CONNECTIONS 4
#define MAX_THREADS 2

using namespace std;

Queue buffer(MAX_CONNECTIONS);
threads thread[MAX_THREADS];

void *Consumer(void *thread) {
	while(1) {
		int client;
		try {
			pthread_mutex_lock(&buffer.rmutex);
			client = buffer.dequeue();
			cout << "Request popped" << endl;
		} catch (QueueEmpty& qe){
			pthread_cond_wait(&buffer.rcond, &buffer.rmutex);
			client = buffer.dequeue();
		}
		pthread_mutex_unlock(&buffer.rmutex);
		pthread_cond_signal(&buffer.wcond);
		imageHandle imgHandle(client);

		try {
			imgHandle.parsePacket();
		} catch (InvalidData& id) {
			cerr << "Failed to retrieve Data from socket: " << client << endl;
		}

		imgHandle.processImage();

		try {
			imgHandle.returnFile();
		} catch (ErrorInReturningImage& er) {
			cerr << "Failed to send data processed Data over socket: " << client << endl;
		}
		close(client);
	}
	return 0;
}

void *Producer(void *thread) {
	threads *threadinfo = (threads *)thread;
	struct sockaddr clientinfo;
	int *retval = new int();
	SocketHandler socketHandle(threadinfo->getData());

	try {
		socketHandle.createSocket();
	}catch(FailedToCreateSocket& soc) {
		*retval = -1;
		pthread_exit((void *)retval);
	}

	try {
		socketHandle.bindSocket();
	} catch (FailedToBindSocket& socBind) {
		socketHandle.closeSocket();
		*retval = -2;
		pthread_exit((void *)retval);
	}

	try {
		socketHandle.listenToSocket();
	}catch(FailedToListen& socListen) {
		socketHandle.closeSocket();
		*retval = -3;
		pthread_exit((void *)retval);
	}
	while(1) {
		int client;
		try {
			client = socketHandle.acceptConnection(&clientinfo);
		} catch(FailedToAccept& fa) {
			cerr << "Something went wrong while accepting connection" << endl;
		}
		try {
			pthread_mutex_lock(&buffer.rmutex);
			buffer.enqueue(client);
			cout << "Request queued" << endl;
		} catch (QueueFull& qf) {
			cerr << "buffer full" << endl;
			pthread_cond_wait(&buffer.wcond, &buffer.rmutex);
			pthread_mutex_unlock(&buffer.rmutex);
			continue;
		}
		pthread_mutex_unlock(&buffer.rmutex);
		pthread_cond_signal(&buffer.rcond);
	}
	socketHandle.closeSocket();
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 1 ) {
		cout << "Usage" << endl;
		cout << "server <port>" << endl;
		exit(-1);
	}

	uint16_t port = atoi(argv[1]);
	thread[0].setData(port);
	pthread_create(thread[0].getPid(), thread[0].getAttr(),Producer,(void *)&thread[0]);

	for(int i = 1;i < MAX_THREADS; i++) {
		pthread_create(thread[i].getPid(), thread[i].getAttr(), Consumer, (void *)&thread[i]);
	}

	for(int i = 1;i < MAX_THREADS;i++) {
		thread[i].setPriorityMin();
	}
	thread[0].setPriorityMax();

	void *ret;
	pthread_join(*thread[0].getPid(),&ret);
	if ((int *)ret < 0) {
		for (int i = 1; i < MAX_THREADS; i++) {
			pthread_cancel(*thread[i].getPid());
		}
	}
	for (int i = 1; i < MAX_THREADS; i++) {
		pthread_join(*thread[0].getPid(),&ret);
	}
	return 0;
}
