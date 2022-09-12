#include <cstdio>
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

Mat src, frame, partial; //第一張圖, 當前幀, 切割
/////mouse movement
void onMouse(int Event, int x, int y, int flags, void* param);
Point pntLU(-1, -1);
Point pntRD(-1, -1);
bool mouseLdown = false, mouseLup = false;
Rect chosen_box;
bool finding = false;
/////tracking
bool tracking = false;
Mat firstSight;//src的SSD
double minVal, fstminVal; //當前最小值 ＆ src 最小值 
Point minLoc, fstminLoc; //對應位置
void diamondSearch(double &centerVal, Point &centerLoc, Mat inputFrame);

int main() {
	VideoCapture video(0);
	if (!video.isOpened()) {
		cout << "Error to Read Video file." << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "succcess gotting the camera." << endl;

	namedWindow("Video displaying", 0);
	setMouseCallback("Video displaying", onMouse, NULL);

	while (true) {
		video.read(frame);
		if (frame.empty())
			break;
		else if (cvWaitKey(33) == 27)
			break;

		if (finding) { // 存起第一張圖
			firstSight.create(src.rows - partial.rows + 1, src.cols - partial.cols + 1, CV_32FC1);
			matchTemplate(src, partial, firstSight, CV_TM_SQDIFF);
			minMaxLoc(firstSight, &minVal, 0, &minLoc, 0);
			Mat result = frame.clone();
			fstminVal = minVal;
			fstminLoc = minLoc;
			finding = false;
			tracking = true;
		}
		else if (tracking) {
			diamondSearch(minVal, minLoc, frame);
			Mat result = frame.clone();
			rectangle(result, minLoc, Point(minLoc.x + partial.cols, minLoc.y + partial.rows), Scalar(0, 0, 255), 3);
			imshow("Video displaying", result);
		}
		else
			imshow("Video displaying", frame);
	}

	return 0;
}

void diamondSearch(double &centerVal, Point &centerLoc, Mat inputFrame)
{
	double refVal = centerVal;
	Point refLoc = centerLoc;

	double diaMinVal, pntMinVal;
	Point diaMinLoc, pntMinLoc;
	Mat compr = inputFrame.clone();
	Rect cut;

	for (int i = 0; i < 9; i++) {
		cut.width = chosen_box.width;	cut.height = chosen_box.height;
		if (i == 0) {
			cut.x = refLoc.x > 0 ? (refLoc.x < (inputFrame.cols - 1) ? refLoc.x : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y + 2) > 0 ? ((refLoc.y + 2) < (inputFrame.rows - 1) ? (refLoc.y + 2) : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 1) {
			cut.x = (refLoc.x + 1) > 0 ? ((refLoc.x + 1) < (inputFrame.cols - 1) ? (refLoc.x + 1) : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y + 1) > 0 ? ((refLoc.y + 1) < (inputFrame.rows - 1) ? (refLoc.y + 1) : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 2) {
			cut.x = (refLoc.x + 2) > 0 ? ((refLoc.x + 2) < (inputFrame.cols - 1) ? (refLoc.x + 2) : (inputFrame.cols - 1)) : 0;
			cut.y = refLoc.y > 0 ? (refLoc.y < (inputFrame.rows - 1) ? refLoc.y : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 3) {
			cut.x = (refLoc.x - 1) > 0 ? ((refLoc.x - 1) < (inputFrame.cols - 1) ? (refLoc.x - 1) : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y + 1) > 0 ? ((refLoc.y + 1) < (inputFrame.rows - 1) ? (refLoc.y + 1) : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 4) {
			cut.x = refLoc.x > 0 ? (refLoc.x < (inputFrame.cols - 1) ? refLoc.x : (inputFrame.cols - 1)) : 0;
			cut.y = refLoc.y > 0 ? (refLoc.y < (inputFrame.rows - 1) ? refLoc.y : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 5) {
			cut.x = (refLoc.x + 1) > 0 ? ((refLoc.x + 1) < (inputFrame.cols - 1) ? (refLoc.x + 1) : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y - 1) > 0 ? ((refLoc.y - 1) < (inputFrame.rows - 1) ? (refLoc.y - 1) : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 6) {
			cut.x = (refLoc.x - 2) > 0 ? ((refLoc.x - 2) < (inputFrame.cols - 1) ? (refLoc.x - 2) : (inputFrame.cols - 1)) : 0;
			cut.y = refLoc.y > 0 ? (refLoc.y < (inputFrame.rows - 1) ? refLoc.y : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 7) {
			cut.x = (refLoc.x - 1) > 0 ? ((refLoc.x - 1) < (inputFrame.cols - 1) ? (refLoc.x - 1) : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y - 1) > 0 ? ((refLoc.y - 1) < (inputFrame.rows - 1) ? (refLoc.y - 1) : (inputFrame.rows - 1)) : 0;
		}
		else if (i == 8) {
			cut.x = refLoc.x > 0 ? (refLoc.x < (inputFrame.cols - 1) ? refLoc.x : (inputFrame.cols - 1)) : 0;
			cut.y = (refLoc.y - 2) > 0 ? ((refLoc.y - 2) < (inputFrame.rows - 1) ? (refLoc.y - 2) : (inputFrame.rows - 1)) : 0;
		}
		Mat crop(compr, cut);

		Mat thisPnt;
		thisPnt.create(partial.rows - crop.rows + 1, partial.cols - crop.cols + 1, CV_32FC1);
		matchTemplate(partial, crop, thisPnt, CV_TM_SQDIFF);
		minMaxLoc(thisPnt, &pntMinVal, 0, &pntMinLoc, 0);

		if (i == 0) {
			diaMinLoc.x = cut.x;
			diaMinLoc.y = cut.y;
			diaMinVal = pntMinVal;
		}
		else {
			if (pntMinVal < diaMinVal) {
				diaMinLoc.x = cut.x;
				diaMinLoc.y = cut.y;
				diaMinVal = pntMinVal;
			}
		}
	}//end for
	//cout << "1" << endl;
	if (diaMinLoc == refLoc) {//center
		for (int i = 0; i < 5; i++) {
			cut.width = chosen_box.width;	cut.height = chosen_box.height;
			if (i == 0) {
				cut.x = refLoc.x > 0 ? refLoc.x : 0;
				cut.y = (refLoc.y + 1) > 0 ? (refLoc.y + 1) : 0;
			}
			else if (i == 1) {
				cut.x = (refLoc.x + 1) < (inputFrame.cols - 1) ? (refLoc.x + 1) : (inputFrame.cols - 1);
				cut.y = refLoc.y > 0 ? refLoc.y : 0;
			}
			else if (i == 2) {
				cut.x = refLoc.x > 0 ? (refLoc.x < (inputFrame.cols - 1) ? refLoc.x : (inputFrame.cols - 1)) : 0;
				cut.y = refLoc.y > 0 ? (refLoc.y < (inputFrame.rows - 1) ? refLoc.y : (inputFrame.rows - 1)) : 0;
			}
			else if (i == 3) {
				cut.x = (refLoc.x - 1) > 0 ? (refLoc.x - 1) : 0;
				cut.y = refLoc.y > 0 ? refLoc.y : 0;
			}
			else if (i == 4) {
				cut.x = refLoc.x < (inputFrame.cols - 1) ? refLoc.x : (inputFrame.cols - 1);
				cut.y =  (refLoc.y-1) > 0 ? ((refLoc.y - 1) < (inputFrame.rows - 1) ? (refLoc.y - 1) : (inputFrame.rows - 1)) : 0;
			}
			Mat crop(compr, cut);

			Mat thisPnt;
			thisPnt.create(partial.rows - crop.rows + 1, partial.cols - crop.cols + 1, CV_32FC1);
			matchTemplate(partial, crop, thisPnt, CV_TM_SQDIFF);
			minMaxLoc(thisPnt, &pntMinVal, 0, &pntMinLoc, 0);

			if (i == 0) {
				diaMinLoc.x = cut.x;
				diaMinLoc.y = cut.y;
				diaMinVal = pntMinVal;
			}
			else {
				if (pntMinVal < diaMinVal) {
					diaMinLoc.x = cut.x;
					diaMinLoc.y = cut.y;;
					diaMinVal = pntMinVal;
				}
			}
		}
		minLoc = diaMinLoc;
		minVal = diaMinVal;
		//cout << "0" << endl;
	}
	else {
		diamondSearch(diaMinVal, diaMinLoc, inputFrame);
	}
}


void onMouse(int Event, int x, int y, int flags, void * param)
{
	if (Event == CV_EVENT_LBUTTONDOWN) { //按下左鍵
		mouseLdown = true;
		tracking = false;
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
		Mat choosing = frame.clone();
		rectangle(choosing, Rect(pntLU, pntTemp), Scalar(255, 0, 0), 3);
		imshow("Video displaying", choosing);
	}
	if (mouseLdown && mouseLup) { //按下後放開(以選取完畢)
		src = frame.clone();
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