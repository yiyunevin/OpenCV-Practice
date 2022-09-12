#include "calibration.h"

void CameraCalibrator::setFilename() {
	m_filenames.clear();
	m_filenames.push_back("chessboard01.jpg");
	m_filenames.push_back("chessboard02.jpg");
	m_filenames.push_back("chessboard03.jpg");
	m_filenames.push_back("chessboard04.jpg");
	m_filenames.push_back("chessboard05.jpg");
	m_filenames.push_back("chessboard06.jpg");
	m_filenames.push_back("chessboard07.jpg");
	m_filenames.push_back("chessboard08.jpg");
	m_filenames.push_back("chessboard09.jpg");
	m_filenames.push_back("chessboard10.jpg");
	m_filenames.push_back("chessboard11.jpg");
	m_filenames.push_back("chessboard12.jpg");
	m_filenames.push_back("chessboard13.jpg");
	m_filenames.push_back("chessboard14.jpg");
}

void CameraCalibrator::setBorderSize(const Size &borderSize) {
	m_borderSize = borderSize;
}

void CameraCalibrator::addChessboardPoints() {
	vector<Point2f> srcCandidateCorners;
	vector<Point3f> dstCandidateCorners;
	for (int i = 0; i < m_borderSize.height; i++) {
		for (int j = 0; j < m_borderSize.width; j++) {
			dstCandidateCorners.push_back(Point3f(i, j, 0.0f));
		}
	}

	for (int i = 0; i < m_filenames.size(); i++) {
		Mat image = imread(m_filenames[i], CV_LOAD_IMAGE_GRAYSCALE);
		findChessboardCorners(image, m_borderSize, srcCandidateCorners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
			+ CALIB_CB_FAST_CHECK);
		TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
		cornerSubPix(image, srcCandidateCorners, Size(5, 5), Size(-1, -1), param);
		if (srcCandidateCorners.size() == m_borderSize.area()) {
			addPoints(srcCandidateCorners, dstCandidateCorners);
		}
	}
}

void CameraCalibrator::addPoints(const vector<Point2f>&imageCorners, const vector<Point3f> &objCorners) {
	m_srcPoints.push_back(imageCorners);
	m_dstPoints.push_back(objCorners);
}

void CameraCalibrator::calibrate(const Mat&src, Mat &dst) {
	Size imageSize = src.size();
	Mat cameraMatrix, distCoeffs, map1, map2;
	vector<Mat> rvecs, tvecs;
	calibrateCamera(m_dstPoints, m_srcPoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
	remap(src, dst, map1, map2, INTER_LINEAR);
}

void CameraCalibrator::drawCorner(Mat&target) {
	vector<Point2f> corners;
	Mat targetGray;
	cvtColor(target, targetGray, CV_BGR2GRAY);
	bool ret = findChessboardCorners(targetGray, m_borderSize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
		+ CALIB_CB_FAST_CHECK);
	TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
	cornerSubPix(targetGray, corners, Size(5, 5), Size(-1, -1), param);
	drawChessboardCorners(target, m_borderSize, corners, ret);
	
	//check
	/*cout << corners[0].x << " " << corners[0].y << endl;
	cout << corners[1].x << " " << corners[1].y << endl;
	circle(target, corners[0], 3, Scalar(0, 255, 0), -1);
	circle(target, corners[1], 3, Scalar(210, 100, 0), -1);*/
}

void CameraCalibrator::drawCube(Mat &src, Mat &dst, Mat &rst)
{
	vector<Point2f> srcCorners;
	vector<Point2f> tempCorners;
	vector<Point3f> dstCorners;

	Mat temp;
	for (int i = 0; i < m_borderSize.height; i++) {
		for (int j = 0; j < m_borderSize.width; j++) {
			dstCorners.push_back(Point3f(i, j, 0.0f));
		}
	}

	findChessboardCorners(dst, m_borderSize, srcCorners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
	TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
	cornerSubPix(dst, srcCorners, Size(5, 5), Size(-1, -1), param);
	
	vector<vector<Point2f>> VsrcCorners;
	vector<vector<Point3f>> VdstCorners;

	VsrcCorners.push_back(srcCorners);
	VdstCorners.push_back(dstCorners);

	//dst src temp ½Õ´«
	Size imageSize = dst.size();
	Mat cameraMatrix, distCoeffs, map1, map2;
	vector<Mat> rvecs, tvecs;
	calibrateCamera(VdstCorners, VsrcCorners, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), imageSize, CV_32F, map1, map2);
	remap(dst, temp, map1, map2, INTER_LINEAR);
	
	//Mat newCameraMatrix = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0, imageSize);
	
	Mat rvecsT;
	Rodrigues(rvecs[0], rvecsT);
	/*cout << rvecs << endl;
	cout << rvecsT << endl;*/

	Mat change1 = cameraMatrix * rvecsT;
	Mat change2 = cameraMatrix * tvecs[0];
	/*cout << change1 << endl;
	cout << change2 << endl;*/
		
	float v1[3] = { 1, 2, 0 };
	float v2[3] = { 1, 2, 1 };
	float v3[3] = { 3, 2, 0 };
	float v4[3] = { 3, 2, 1 };
	float v5[3] = { 1, 4, 0 };
	float v6[3] = { 1, 4, 1 };
	float v7[3] = { 3, 4, 0 };
	float v8[3] = { 3, 4, 1 };

	float x1, y1, z1;
	x1 = change1.at<double>(0, 0)*v1[0] + change1.at<double>(0, 1)*v1[1] + change1.at<double>(0, 2)*v1[2] + change2.at<double>(0, 0);
	y1 = change1.at<double>(1, 0)*v1[0] + change1.at<double>(1, 1)*v1[1] + change1.at<double>(1, 2)*v1[2] + change2.at<double>(1, 0);
	z1 = change1.at<double>(2, 0)*v1[0] + change1.at<double>(2, 1)*v1[1] + change1.at<double>(2, 2)*v1[2] + change2.at<double>(2, 0);
	x1 /= z1;
	y1 /= z1;
	cout << x1 << "," << y1 << endl;
	float x2, y2, z2;
	x2 = change1.at<double>(0, 0)*v2[0] + change1.at<double>(0, 1)*v2[1] + change1.at<double>(0, 2)*v2[2] + change2.at<double>(0, 0);
	y2 = change1.at<double>(1, 0)*v2[0] + change1.at<double>(1, 1)*v2[1] + change1.at<double>(1, 2)*v2[2] + change2.at<double>(1, 0);
	z2 = change1.at<double>(2, 0)*v2[0] + change1.at<double>(2, 1)*v2[1] + change1.at<double>(2, 2)*v2[2] + change2.at<double>(2, 0);
	x2 /= z2;
	y2 /= z2;
	cout << x2 << "," << y2  << endl;

	float x3, y3, z3;
	x3 = change1.at<double>(0, 0)*v3[0] + change1.at<double>(0, 1)*v3[1] + change1.at<double>(0, 2)*v3[2] + change2.at<double>(0, 0);
	y3 = change1.at<double>(1, 0)*v3[0] + change1.at<double>(1, 1)*v3[1] + change1.at<double>(1, 2)*v3[2] + change2.at<double>(1, 0);
	z3 = change1.at<double>(2, 0)*v3[0] + change1.at<double>(2, 1)*v3[1] + change1.at<double>(2, 2)*v3[2] + change2.at<double>(2, 0);
	x3 /= z3;
	y3 /= z3;
	cout << x3 << "," << y3 << endl;

	float x4, y4, z4;
	x4 = change1.at<double>(0, 0)*v4[0] + change1.at<double>(0, 1)*v4[1] + change1.at<double>(0, 2)*v4[2] + change2.at<double>(0, 0);
	y4 = change1.at<double>(1, 0)*v4[0] + change1.at<double>(1, 1)*v4[1] + change1.at<double>(1, 2)*v4[2] + change2.at<double>(1, 0);
	z4 = change1.at<double>(2, 0)*v4[0] + change1.at<double>(2, 1)*v4[1] + change1.at<double>(2, 2)*v4[2] + change2.at<double>(2, 0);
	x4 /= z4;
	y4 /= z4;

	float x5, y5, z5;
	x5 = change1.at<double>(0, 0)*v5[0] + change1.at<double>(0, 1)*v5[1] + change1.at<double>(0, 2)*v5[2] + change2.at<double>(0, 0);
	y5 = change1.at<double>(1, 0)*v5[0] + change1.at<double>(1, 1)*v5[1] + change1.at<double>(1, 2)*v5[2] + change2.at<double>(1, 0);
	z5 = change1.at<double>(2, 0)*v5[0] + change1.at<double>(2, 1)*v5[1] + change1.at<double>(2, 2)*v5[2] + change2.at<double>(2, 0);
	x5 /= z5;
	y5 /= z5;

	float x6, y6, z6;
	x6 = change1.at<double>(0, 0)*v6[0] + change1.at<double>(0, 1)*v6[1] + change1.at<double>(0, 2)*v6[2] + change2.at<double>(0, 0);
	y6 = change1.at<double>(1, 0)*v6[0] + change1.at<double>(1, 1)*v6[1] + change1.at<double>(1, 2)*v6[2] + change2.at<double>(1, 0);
	z6 = change1.at<double>(2, 0)*v6[0] + change1.at<double>(2, 1)*v6[1] + change1.at<double>(2, 2)*v6[2] + change2.at<double>(2, 0);
	x6 /= z6;
	y6 /= z6;

	float x7, y7, z7;
	x7 = change1.at<double>(0, 0)*v7[0] + change1.at<double>(0, 1)*v7[1] + change1.at<double>(0, 2)*v7[2] + change2.at<double>(0, 0);
	y7 = change1.at<double>(1, 0)*v7[0] + change1.at<double>(1, 1)*v7[1] + change1.at<double>(1, 2)*v7[2] + change2.at<double>(1, 0);
	z7 = change1.at<double>(2, 0)*v7[0] + change1.at<double>(2, 1)*v7[1] + change1.at<double>(2, 2)*v7[2] + change2.at<double>(2, 0);
	x7 /= z7;
	y7 /= z7;

	float x8, y8, z8;
	x8 = change1.at<double>(0, 0)*v8[0] + change1.at<double>(0, 1)*v8[1] + change1.at<double>(0, 2)*v8[2] + change2.at<double>(0, 0);
	y8 = change1.at<double>(1, 0)*v8[0] + change1.at<double>(1, 1)*v8[1] + change1.at<double>(1, 2)*v8[2] + change2.at<double>(1, 0);
	z8 = change1.at<double>(2, 0)*v8[0] + change1.at<double>(2, 1)*v8[1] + change1.at<double>(2, 2)*v8[2] + change2.at<double>(2, 0);
	x8 /= z8;
	y8 /= z8;
		
	cvtColor(dst, rst, CV_GRAY2BGR);
	line(rst, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 2);
	line(rst, Point(x3, y3), Point(x4, y4), Scalar(0, 0, 255), 2);
	line(rst, Point(x5, y5), Point(x6, y6), Scalar(0, 0, 255), 2);
	line(rst, Point(x7, y7), Point(x8, y8), Scalar(0, 0, 255), 2);
	line(rst, Point(x1, y1), Point(x3, y3), Scalar(0, 0, 255), 2);
	line(rst, Point(x1, y1), Point(x5, y5), Scalar(0, 0, 255), 2);
	line(rst, Point(x3, y3), Point(x7, y7), Scalar(0, 0, 255), 2);
	line(rst, Point(x5, y5), Point(x7, y7), Scalar(0, 0, 255), 2);
	line(rst, Point(x2, y2), Point(x4, y4), Scalar(0, 0, 255), 2);
	line(rst, Point(x2, y2), Point(x6, y6), Scalar(0, 0, 255), 2);
	line(rst, Point(x4, y4), Point(x8, y8), Scalar(0, 0, 255), 2);
	line(rst, Point(x6, y6), Point(x8, y8), Scalar(0, 0, 255), 2);
}


