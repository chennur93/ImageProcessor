#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "imageManagement.hpp"

using namespace std;
using namespace cv;

imageHandle::imageHandle(){
	//size = 0;
	command = 0;
	file = "";
	socketfd = 0;
	serverAddress = NULL;
	processedFilePath = "";
}

imageHandle::imageHandle(uint8_t command,string filePath){
	this->command = command;
	this->file = filePath;
	socketfd = 0;
	serverAddress = NULL;
	processedFilePath = "";
}

void imageHandle::connectToServer(char* host, char* port) {
	struct addrinfo hostinfo;
	bzero(&hostinfo,sizeof(hostinfo));
	hostinfo.ai_family = AF_INET;
	hostinfo.ai_socktype = SOCK_STREAM;
	hostinfo.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(host,port,&hostinfo,&serverAddress) != 0){
		printf("ERROR: %s\n", strerror(errno));
		freeaddrinfo(serverAddress);
		exit(-2);
	}

	if ((socketfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1){
		printf("ERROR: %s\n", strerror(errno));
		freeaddrinfo(serverAddress);
		exit(-3);
	}

	if (connect(socketfd,serverAddress->ai_addr,serverAddress->ai_addrlen) < 0){
		printf("ERROR: %s\n", strerror(errno));
		freeaddrinfo(serverAddress);
		exit(-4);
	}

}

string imageHandle::getFile() {
	return file;
}

void imageHandle::setFile(string filePath) {
	this->file = filePath;
}

void imageHandle::setCommand(uint8_t command) {
	this->command = command;
}

uint8_t imageHandle::getCommand() {
	return command;
}

imageHandle::~imageHandle() {
	freeaddrinfo(serverAddress);
}

void imageHandle::sendFileToServer() {
	char header[20];
	uint32_t length;
	memcpy(header,(void *)"Command:",8);
	memcpy(header + 8,(void *)&command,1);
	memcpy(header + 9,(void *)"\nsize:",6);
	fstream fp;
	fp.open(file,ios::binary | ios::in);
	fp.seekg(0,fp.end);
	length = fp.tellg();
	fp.seekg(0,fp.beg);
	memcpy(header + 15,(void *)&length,4);
	memcpy(header + 19,(void *)"\0",1);
	if (send(socketfd,header,20,0) != 20) {
		exit(-5);
	}
	char *buf = new char[length];
	fp.read(buf,length);
	for (uint32_t i = 0;i < length; ) {
		int size;
		if (length - i > 1500) {
			size = send(socketfd,buf+i,1500,0);
		} else {
			size =  send(socketfd,buf+i,length - i,0);
		}
		i += size;
	}
	fp.close();
	delete buf;
}

void imageHandle::receiveProcessedFile() {
	char header[10];
	char *buf;
	uint32_t size;
	if ((size = recv(socketfd,header,10,0)) < 0) {
		cerr << "Failed to receive Data with ERR: " << size << endl;
		exit(-100);
	}
	header[10] = '\0';
	memcpy(&size,header + 5,4);
	buf = new char[size];
	for (uint32_t i = 0; i < size;) {
		int temp;
		if ((temp = recv(socketfd,buf + i,1500,0)) < 0) {
			cerr << "Failed to receive Data with ERR: " << size << endl;
			exit(-10);
		}
		i += temp;
	}
	switch(command) {
	case 0: processedFilePath = file.replace(file.end() - 4, file.end(), " - gray.jpg");
			break;

	case 1: processedFilePath = file.replace(file.end() - 4, file.end(), " - yuv.jpg");
			break;

	case 2: processedFilePath = file.replace(file.end() - 4, file.end(), " - hsv.jpg");
			break;

	case 3: processedFilePath = file.replace(file.end() - 4, file.end(), " - bw.jpg");
			break;

	case 4: processedFilePath = file.replace(file.end() - 4, file.end(), " - random.jpg");
			break;

	default:break;
	}
	fstream fp;
	fp.open(processedFilePath,ios::out | ios::binary);
	fp.write(buf, size);
	fp.close();
	delete buf;
}

void imageHandle::showOriginalImage() {
	Mat image = imread(file,1);
	if (!image.data) {
		cout << "Invalid image!" << endl;
		exit(-2);
	}
	namedWindow(file, WINDOW_AUTOSIZE);
	imshow(file,image);

	waitKey(50);
}

void imageHandle::showProcessedImage() {
	Mat image = imread(processedFilePath,1);
	if (!image.data) {
		cout << "Invalid image!" << endl;
		exit(-2);
	}
	namedWindow(processedFilePath, WINDOW_AUTOSIZE);
	imshow(processedFilePath,image);

	waitKey(50);
}
