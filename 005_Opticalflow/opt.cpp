#include <cstdio>
#include <opencv2\opencv.hpp>
#include <opencv2\features2d.hpp>
#include <opencv2\video.hpp>
using namespace cv;
using namespace std;

int main() {
	///***open the video file
	VideoCapture video("featureTracking.mp4");
	if (!video.isOpened()) {
		cout << "Error to Read Video file." << endl;
		exit(EXIT_FAILURE);
	}
	else
		cout << "succcess gotting the camera." << endl;

	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	vector<Point2f> tracking, prePoints, findingPoints;
	Mat frame;

	///***first frame => blob detector
	video.read(frame);
	Mat blobDet = frame.clone();
	/////parameter setting
	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 1.0f;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.4;
	params.filterByConvexity = true;
	params.minConvexity = 0.87;
	params.filterByColor = true;
	params.blobColor = 0;
	params.filterByCircularity = true;
	params.minCircularity = 0.83;
	params.filterByArea = true;
	params.minArea = 30.0f;
	params.maxArea = 500.0f;
	/////find blobs
	Ptr<SimpleBlobDetector> blobDetector = SimpleBlobDetector::create(params);
	vector<KeyPoint>blobFeatures;
	blobDetector -> detect(blobDet, blobFeatures);
	/////show blobs
	drawKeypoints(blobDet, blobFeatures, blobDet, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("Blob Detect", blobDet);

	for (int i = 0; i < blobFeatures.size(); i++) {
		prePoints.push_back(blobFeatures[i].pt);
		tracking.push_back(blobFeatures[i].pt);
	}
	///***tracking => optical flow LK
	Size winSize(21, 21);
	namedWindow("Video displaying", 0);
	//first => for comparison
	Mat opLKfst = frame.clone();

	while (true) {
		//read frames
		video.read(frame);
		if (frame.empty())
			break;
		else if (cvWaitKey(33) == 27)
			break;
		//second => current target
		Mat opLKsnd = frame.clone();
		vector<uchar> status;
		vector<float> err;
		calcOpticalFlowPyrLK(opLKfst, frame, prePoints, findingPoints, status, err, winSize, 3, termcrit, 0, 0.001);
		for (int i = 1; i < tracking.size(); ++i)
			circle(opLKsnd, tracking[i], 1, Scalar(255, 0, 0), -1, 8);
		for (int i = 0; i < findingPoints.size(); ++i) {
			tracking.push_back(findingPoints[i]);
			circle(opLKsnd, findingPoints[i], 4, Scalar(0, 255, 0), -1, 8);
		}
		std::swap(prePoints, findingPoints);
		cv::swap(opLKfst, frame);
		imshow("Video displaying", opLKsnd);
	}
	waitKey(0);
	return 0;
}