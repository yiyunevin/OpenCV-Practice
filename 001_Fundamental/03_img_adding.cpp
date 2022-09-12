/*同樣大小的圖片相加*/
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	Mat src1 = imread("potan.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat src2 = imread("color.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat addjust;

	addWeighted(src1, 0.5, src2, 0.5, -50, addjust);
	//兩圖片尺寸必須相同
	//圖片1, 圖1權重, 圖片2, 圖2權重, 相加後再調整的值（飽和度）, 輸出圖

	namedWindow("additive result");
	imshow("additive result", addjust);
	
	waitKey(0);

	return 0;
}
