/*ø�s�X��*/
#include <cstdio>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;	//�_�h�L�k���� string

int main() {
	Mat img(400, 500, CV_8UC3, Scalar(255, 255, 255));
	line(img, Point(20, 40), Point(120, 140), Scalar(255, 0, 0), 3);
	rectangle(img, Point(150, 40), Point(250, 140), Scalar(0, 0, 255), -1);
	circle(img, Point(330, 90), 50, Scalar(0, 255, 0), -1);
	ellipse(img, Point(80, 280), Size(60, 40), 45, 0, 360, Scalar(255, 255, 0), 2);
	//�e���A��ߡA�ؤo�A���ਤ�סA���װ_�l���סA�������סA�C��A�u�e�^

	//�h���
	Point points[1][5];
	points[0][0] = Point(150, 270);
	points[0][1] = Point(190, 220);
	points[0][2] = Point(260, 255);
	points[0][3] = Point(224, 296);
	points[0][4] = Point(178, 316);
	const Point* ppt[1] = { points[0] };
	int npt[] = { 5 };	//���I��
	polylines(img, ppt, npt, 1, 1, Scalar(0, 255, 255), 5);
	//�e���A���u�I�}�C�A���I�ư}�C�A���u�ơ]�n�������H�^�A�ʳ��P�_�A�C��A��u�e�ס]�t��/CV_FILLED��ܶ񺡡^
	
	putText(img, string("Yeahhhhhh"), Point(280, 280), 0, 1, Scalar(0, 0, 0), 3);
	//���䴩����
	//point�G���U���y�СA�r��˦��A�r��j�p

	imshow("Shapes!", img);
	waitKey(0);

	return 0;
}