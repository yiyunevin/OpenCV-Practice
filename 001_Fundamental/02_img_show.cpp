/*從硬碟直接讀去檔案顯示於視窗上（彩色），當按下esc時程式終止、視窗關閉*/
/*
//cvwaitKey(int delay = 0)  1. 只有搭配OPCV視窗才有作用
//							2. esc鍵的ASCII碼 = 27
//							3. delay <= 0：程式靜止
//							   delay >  0：等待？ms後返回按鍵的ASCII碼 
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