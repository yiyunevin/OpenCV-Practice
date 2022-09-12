#include "calibration.h"

int main() {
	CameraCalibrator myCameraCalibrator;
	myCameraCalibrator.setFilename();
	myCameraCalibrator.setBorderSize(Size(6, 4));
	
	myCameraCalibrator.addChessboardPoints();

	Mat src = imread("chessboard09.jpg", 0);
	Mat dst, corner, cube;
	myCameraCalibrator.calibrate(src, dst);

	cvtColor(dst, corner, CV_GRAY2BGR);
	cvtColor(dst, cube, CV_GRAY2BGR);

	myCameraCalibrator.drawCorner(corner);
	myCameraCalibrator.drawCube(src, dst, cube);

	imshow("Original Image", src);
	imshow("Undistorted Image", dst);
	imshow("Draw Corners", corner);
	imshow("Draw Cube", cube);
	
	waitKey();

	return 0;
}