///// == 滑鼠擷取圖片部分區域 == /////
#include <cstdio>
#include <opencv2\opencv.hpp>
using namespace cv;

Mat src, partial;
void onMouse(int Event, int x, int y, int flags, void* param);
Point pntLU(-1, -1);
Point pntRD(-1, -1);
bool mouseLdown = false, mouseLup = false;
Rect chosen_box;
bool finding = false;

int main() {
	src = imread("chessboard13.jpg", CV_LOAD_IMAGE_UNCHANGED);
	namedWindow("Origin", 0);
	setMouseCallback("Origin", onMouse, NULL);
	imshow("Origin", src);

	while (true) {
		if (cvWaitKey(33) == 27) {
			break;
		}
		if (finding) {
			Mat findSame;
			findSame.create(src.rows - partial.rows + 1, src.cols - partial.cols + 1, CV_32FC1);
			matchTemplate(src, partial, findSame, CV_TM_SQDIFF);
			double minVal;
			Point minLoc;
			minMaxLoc(findSame, &minVal, 0, &minLoc, 0);
			Mat result = src.clone();
			rectangle(result, minLoc, Point(minLoc.x + partial.cols, minLoc.y + partial.rows), Scalar(0, 0, 255), 3);
			imshow("Finding", result);
		}
	}

	return 0;
}

void onMouse(int Event, int x, int y, int flags, void * param)
{
	if (Event == CV_EVENT_LBUTTONDOWN) { //按下左鍵
		mouseLdown = true;
		finding = false;
		pntLU.x = x;
		pntLU.y = y;
	}
	if (Event == CV_EVENT_LBUTTONUP) { //放開左鍵
		mouseLup = true;
		pntRD.x = x;
		pntRD.y = y;
	}
	if (mouseLdown && !mouseLup) { //左鍵按住不放 -> 顯示選取範圍
		Point pntTemp;
		pntTemp.x = x;
		pntTemp.y = y;
		Mat choosing = src.clone();
		rectangle(choosing, Rect(pntLU, pntTemp), Scalar(255, 0, 0), 3);
		imshow("Origin", choosing);
	}
	if (mouseLdown &&mouseLup) { //按下後放開(以選取完畢)
		chosen_box.width = abs(pntLU.x - pntRD.x);
		chosen_box.height = abs(pntLU.y - pntRD.y);
		chosen_box.x = min(pntLU.x, pntRD.x);
		chosen_box.y = min(pntLU.y, pntRD.y);
		Mat crop(src, chosen_box);
		namedWindow("Cropped_Image");
		imshow("Cropped_Image", crop);
		partial = crop.clone();
		mouseLdown = false;
		mouseLup = false;
		finding = true;
	}
}