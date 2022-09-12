/*�ۥ[��ؤo���P���Ϥ�*/
#include <cstdio>
#include <opencv2\opencv.hpp>
using namespace cv;

int main() {
	Mat botton = imread("color.png", CV_LOAD_IMAGE_UNCHANGED);
	Mat logo = imread("potan_logo.png", CV_LOAD_IMAGE_UNCHANGED);

	Mat imageROI = botton(Rect(300, 300, logo.cols, logo.rows));
	//�����B�n�������A�b "botton.png" �W����m
	//���W����x�y��, ���W����y�y��, �e, ��

	addWeighted(imageROI, 0, logo, 1.0, 0, imageROI);
	//��b���B�n��������, ��J���Ϥ������P�B�n�ؤo�ۦP

	namedWindow("Display");
	imshow("Display", botton);

	waitKey(0);

	return 0;
}