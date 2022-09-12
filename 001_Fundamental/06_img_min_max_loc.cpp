/*尋找一維陣列(Mat)中最大/小值的位置*/
#include <cstdio>
#include <opencv2\opencv.hpp>

using namespace cv;

int main() {
	Mat src = imread("potan.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat comp = imread("potan_cut.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat displaying = src.clone();
	Mat result;
	result.create(src.rows - comp.rows + 1, src.cols - comp.cols + 1, CV_32FC1);//※

	matchTemplate(src, comp, result, CV_TM_SQDIFF);
	double minVal;
	Point minLoc;
	minMaxLoc(result, &minVal, 0, &minLoc, 0);

	rectangle(displaying, minLoc, Point(minLoc.x + comp.cols, minLoc.y + comp.rows), Scalar::all(0), 5);

	imshow("result", displaying);

	waitKey(0);
	
	return 0;
}