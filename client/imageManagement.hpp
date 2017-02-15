#ifndef SRC_IMAGEMANAGEMENT_HPP_
#define SRC_IMAGEMANAGEMENT_HPP_

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

using namespace std;

class imageHandle {

private:
	uint8_t command;
	string file;
	string processedFilePath;
	int socketfd;
	struct addrinfo *serverAddress;

public:
	imageHandle();

	imageHandle(uint8_t command,string filePath);

	string getFile();

	void setFile(string filePath);

	void setCommand(uint8_t command);

	uint8_t getCommand();

	void connectToServer(char* host, char* port);

	void sendFileToServer();

	void receiveProcessedFile();

	void showOriginalImage();

	void showProcessedImage();

	~imageHandle();
};



#endif /* SRC_IMAGEMANAGEMENT_HPP_ */
