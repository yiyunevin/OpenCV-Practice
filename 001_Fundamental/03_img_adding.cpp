/*�P�ˤj�p���Ϥ��ۥ[*/
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
	Mat src1 = imread("potan.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat src2 = imread("color.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat addjust;

	addWeighted(src1, 0.5, src2, 0.5, -50, addjust);
	//��Ϥ��ؤo�����ۦP
	//�Ϥ�1, ��1�v��, �Ϥ�2, ��2�v��, �ۥ[��A�վ㪺�ȡ]���M�ס^, ��X��

	namedWindow("additive result");
	imshow("additive result", addjust);
	
	waitKey(0);

	return 0;
}
