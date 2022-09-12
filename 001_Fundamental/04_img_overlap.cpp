/*相加兩尺寸不同的圖片*/
#include <cstdio>
#include <opencv2\opencv.hpp>
using namespace cv;

int main() {
	Mat botton = imread("color.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat logo = imread("potan_logo.png", CV_LOAD_IMAGE_UNCHANGED);

	Mat imageROI = botton(Rect(300, 300, logo.cols, logo.rows));
	//類似遮罩的概念，在 "botton.png" 上的位置
	//左上角的x座標, 左上角的y座標, 寬, 高

	addWeighted(imageROI, 0, logo, 1.0, 0, imageROI);
	//放在此遮罩中的物件, 放入之圖片必須與遮罩尺寸相同

	namedWindow("Display");
	imshow("Display", botton);

	waitKey(0);

	return 0;
}