/*�q�w�Ъ���Ū�h�ɮ���ܩ�����W�]�m��^�A����Uesc�ɵ{���פ�B��������*/
/*
//cvwaitKey(int delay = 0)  1. �u���f�tOPCV�����~���@��
//							2. esc�䪺ASCII�X = 27
//							3. delay <= 0�G�{���R��
//							   delay >  0�G���ݡHms���^���䪺ASCII�X 
*/
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
	Mat img = imread("potan.png", CV_LOAD_IMAGE_COLOR);
	while (true) {
		imshow("Display window", img);
		if (cvWaitKey(10) == 27)			//esc
			break;
	}
	return 0;
}