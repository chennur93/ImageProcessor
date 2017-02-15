#ifndef SRC_IMAGEMANAGEMENT_HPP_
#define SRC_IMAGEMANAGEMENT_HPP_

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
#include <opencv2/opencv.hpp>
#include "queue.hpp"
#include "threads.hpp"

using namespace std;

typedef enum THREADSTATUS {EMPTY, INACTIVE, ACTIVE} THREADSTATUS;
class imageHandle {

private:
	//long long size;
	uint8_t command;
	char *file;
	vector<uint8_t> cvtfile;
	int client;
	uint32_t size;

	void grayScale();

public:
	imageHandle();

	imageHandle(int client);

	string getFile();

	void setClient(int client);

	uint8_t getCommand();

	void processImage();

	void returnFile();

	void parsePacket();

	void YUVConvert();

	void HSVConvert();

	void threshhold();

	~imageHandle();
};

class InvalidData : public exception{};
class ErrorInReturningImage : public exception{};

#endif /* SRC_IMAGEMANAGEMENT_HPP_ */
