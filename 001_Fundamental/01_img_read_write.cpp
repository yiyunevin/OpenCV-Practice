/*Ū���ɮ��ର�Ƕ�����ܩ�s�ص����W*/
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
	Mat img = imread("potan.png", CV_LOAD_IMAGE_GRAYSCALE);		//Ū�ɡG��bproject��Ƨ���
	imwrite("grayscale_output.jpg", img);	//��X�ɮסG�W�٩M�ӷ�

	namedWindow("Display window", WINDOW_AUTOSIZE);	//�s�ص����G�����W�١�SIZE
	imshow("Display window", img);	//��ܦb�����W���Ϥ�
	waitKey(0);	//�����N�䵲��, �i�H���ϯd�����W����

	return 0;
}