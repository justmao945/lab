#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	String imageName("eg.jpg");
	if(argc > 1) {
		imageName = argv[1];
	}
	Mat image;
	image = imread(imageName, IMREAD_COLOR);
	if(image.empty()) {
		cout << "count not open image" << endl;
		return -1;
	}
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image);
	waitKey(0);
	return 0;
}
