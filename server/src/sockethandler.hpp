#ifndef SRC_SOCKETHANDLER_HPP_
#define SRC_SOCKETHANDLER_HPP_

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#define MAX_KERNEL_BUFFER 5

using namespace std;

class SocketHandler {
private:
	int socketfd;
	struct sockaddr_in hostinfo;

public:
	SocketHandler(int port);

	void createSocket();

	void bindSocket();

	void listenToSocket();

	int acceptConnection(struct sockaddr *clientinfo);

	void closeSocket();

};


class FailedToCreateSocket : public exception{};
class FailedToBindSocket : public exception{};
class FailedToListen : public exception{};
class FailedToAccept : public exception{};

#endif /* SRC_SOCKETHANDLER_HPP_ */
