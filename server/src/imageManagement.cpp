#include "imageManagement.hpp"

using namespace std;
using namespace cv;

#define HEIGHT 177
#define WIDTH 284

imageHandle::imageHandle(int client){
	//size = 0;
	command = 0;
	size = 0;
	file = NULL;
	this->client = client;
	cvtfile = vector<uint8_t>();
}

imageHandle::imageHandle(){
	//size = 0;
	command = 0;
	size = 0;
	file = NULL;
	this->client = 0;
	cvtfile = vector<uint8_t>();
}

string imageHandle::getFile() {
	return file;
}

void imageHandle::setClient(int client) {
	this->client = client;
}

uint8_t imageHandle::getCommand() {
	return command;
}

imageHandle::~imageHandle() {
	delete file;
}

void imageHandle::parsePacket() {
	char header[20];
	char *buf;
	uint32_t size;
	if ((size = recv(client,header,20,0)) < 0) {
		cerr << "Failed to receive Data with ERR: " << size << endl;
		throw InvalidData();
	}
	header[20] = '\0';
	this->command = header[9];
	memcpy(&this->command,header + 8,1);
	memcpy(&this->size,header + 15,4);
	buf = new char[this->size];
	for (uint32_t i = 0; i < this->size;) {
		int temp;
		if ((temp = recv(client,buf + i,1500,0)) < 0) {
			cerr << "Failed to receive Data with ERR: " << size << endl;
			throw InvalidData();
		}
		i += temp;
	}
	this->file = buf;
}


void imageHandle::processImage() {
	usleep(5000000);
	int command = this->command;
	if (this->command == 4) {
		command = random() % 4;
	}
	switch(command) {
	case 0: grayScale();
			break;

	case 1: YUVConvert();
			break;

	case 2: HSVConvert();
			break;

	case 3: threshhold();
			break;

	default: break;
	}
}

void imageHandle::threshhold() {
	Mat image = Mat(1,size,CV_8UC1,file);
	Mat decoded_image = imdecode(image,1);
	if ( !image.data ) {
    	cout << "No image data \n";
	} else {
		Mat gray_image;
		Mat bw_image;
		cvtColor( decoded_image, gray_image, CV_BGR2GRAY );
		threshold(gray_image, bw_image, 128, 255, CV_THRESH_BINARY);
		vector<int> encodesettings;
		encodesettings.push_back(IMWRITE_JPEG_QUALITY);
		imencode(".jpg", bw_image, cvtfile,encodesettings);
	}
}

void imageHandle::HSVConvert() {
	Mat image = Mat(1,size,CV_8UC1,file);
	Mat decoded_image = imdecode(image,1);
	if ( !image.data ) {
    	cout << "No image data \n";
	} else {
		Mat hsv_image;
		cvtColor( decoded_image, hsv_image, CV_BGR2HSV );
		vector<int> encodesettings;
		encodesettings.push_back(IMWRITE_JPEG_QUALITY);
		imencode(".jpg", hsv_image, cvtfile,encodesettings);
	}
}

void imageHandle::YUVConvert() {
	Mat image = Mat(1,size,CV_8UC1,file);
	Mat decoded_image = imdecode(image,1);
	if ( !image.data ) {
    	cout << "No image data \n";
	} else {
		Mat yuv_image;
		cvtColor( decoded_image, yuv_image, CV_BGR2YUV );
		vector<int> encodesettings;
		encodesettings.push_back(IMWRITE_JPEG_QUALITY);
		imencode(".jpg", yuv_image, cvtfile,encodesettings);
	}
}

void imageHandle::grayScale() {
	Mat image = Mat(1,size,CV_8UC1,file);
	Mat decoded_image = imdecode(image,1);
	if ( !decoded_image.data ) {
    	cout << "No image data \n";
	} else {
		Mat gray_image;
		cvtColor( decoded_image, gray_image, CV_BGR2GRAY );
		vector<int> encodesettings;
		encodesettings.push_back(IMWRITE_JPEG_QUALITY);
		imencode(".jpg", gray_image, cvtfile,encodesettings);
	}
}

void imageHandle::returnFile() {
	char header[10];
	uint32_t length = cvtfile.size();
	memcpy(header,(void *)"size:",5);
	memcpy(header + 5,(void *)&length,4);
	memcpy(header + 4,(void *)"\0",1);
	if (send(this->client,header,10,0) != 10) {
		exit(-5);
	}
	uint8_t *buf = cvtfile.data();
	for (uint32_t i = 0;i < length; ) {
		int size;
		if (length - i > 1500) {
			size = send(this->client,buf+i,1500,0);
		} else {
			size =  send(this->client,buf+i,length - i,0);
		}
		i += size;
	}
}
