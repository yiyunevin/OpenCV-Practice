#include <opencv2\opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int main() {
	Mat src = imread("lena.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Mat comp = imread("lena_mask.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Mat displaying = src.clone();
	Mat result = Mat::zeros(Size(src.cols - comp.cols + 1, src.rows - comp.rows + 1), CV_32FC1);//row<->col?

	matchTemplate(src, comp, result, CV_TM_SQDIFF);
	double minVal;
	Point minLoc;
	minMaxLoc(result, &minVal, 0, &minLoc, 0);
	rectangle(displaying, minLoc, Point(minLoc.x + comp.cols, minLoc.y + comp.rows), Scalar::all(0), 5);
	imshow("looking for", comp);
	imshow("result", displaying);

	waitKey(0);

	return 0;
}