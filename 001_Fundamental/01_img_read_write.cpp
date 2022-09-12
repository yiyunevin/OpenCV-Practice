/*讀取檔案轉為灰階並顯示於新建視窗上*/
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
	Mat img = imread("potan.png", CV_LOAD_IMAGE_GRAYSCALE);		//讀檔：放在project資料夾內
	imwrite("grayscale_output.jpg", img);	//輸出檔案：名稱和來源

	namedWindow("Display window", WINDOW_AUTOSIZE);	//新建視窗：視窗名稱＆SIZE
	imshow("Display window", img);	//顯示在視窗上的圖片
	waitKey(0);	//按任意鍵結束, 可以讓圖留住不馬上關掉

	return 0;
}