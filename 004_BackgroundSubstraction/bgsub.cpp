#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/video.hpp>

using namespace cv;
using namespace std;

int main() {
	Ptr<BackgroundSubtractor> fMOG2;
	Mat frame;
	Mat fgmask;
	//MOG2 ya KNN
	fMOG2 = createBackgroundSubtractorMOG2();

	//VideoCapture video("bgSub.mp4");
	VideoCapture video(0);
	cout << "get cam" << endl;
	if (!video.isOpened()) {
		cout << "Error to Read Video file." << endl;
		exit(EXIT_FAILURE);
	}
	while(true)
	{
		video.read(frame);
		if (frame.empty()) {
			break;
		}
		fMOG2->apply(frame, fgmask);
		imshow("Original Vedio", frame);
		imshow("Background Subtractor", fgmask);	   
		waitKey(30);
	}
	return 0;
}