#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
	Mat image = imread("./images.jpg",1);
	
	namedWindow("images.jpg", WINDOW_AUTOSIZE);
	imshow("images.jpg",image);

	waitKey(0);

	return 0;
}
