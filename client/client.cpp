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
#include <opencv2/opencv.hpp>
#include "imageManagement.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	if (argc < 5 ) {
		cout << "Usage" << endl;
		cout << "client <file path> <command> <host> <port>" << endl;
		exit(-1);
	}

	char *host = argv[3],*port = argv[4];

	string imagePath = argv[1];

	uint8_t command = atoi(argv[2]);

	imageHandle tcpstream(command,imagePath);

	tcpstream.connectToServer(host, port);

	tcpstream.showOriginalImage();

	tcpstream.sendFileToServer();

	tcpstream.receiveProcessedFile();

	tcpstream.showProcessedImage();

	waitKey(0);

	return 0;
}
