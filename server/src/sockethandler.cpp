#include "sockethandler.hpp"

using namespace std;

SocketHandler::SocketHandler(int port) {
	socketfd = 0;
	bzero(&hostinfo,sizeof(hostinfo));
	hostinfo.sin_family = AF_INET;
	hostinfo.sin_port = htons(port);
	hostinfo.sin_addr.s_addr = htonl(INADDR_ANY);
}

void SocketHandler::createSocket() {
	if ((socketfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
		cerr << "Failed to create a socket" << endl;
		throw FailedToCreateSocket();
	}
}

void SocketHandler::bindSocket() {
	if(bind(socketfd,(struct sockaddr *)&hostinfo,sizeof(hostinfo))<0){
		cerr << "Failed to bind socket to port: " << ntohs(hostinfo.sin_port) << endl;
		throw FailedToBindSocket();
	}
}

void SocketHandler::listenToSocket() {
	if (listen(socketfd,MAX_KERNEL_BUFFER) < 0){
		cerr << "Cannot poll on the given socket" << endl;
		throw FailedToListen();
	}
}

int SocketHandler::acceptConnection(struct sockaddr *clientinfo) {
	unsigned int clientlen = sizeof(struct sockaddr_in);
	int client = accept(socketfd,clientinfo,&clientlen);
	if (client == -1){
		cerr << "Failed to create thread, ERR: " << client << endl;
		throw FailedToAccept();
	}
	return client;
}

void SocketHandler::closeSocket() {
	close(socketfd);
}



