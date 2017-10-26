#include "stdafx.h"
#include "time.h"

#include <opencv2/opencv.hpp> // インクルードファイル指定
//#include <opencv2/videoio/>
//#include <opencv2/opencv_lib.hpp> // 静的リンクライブラリの指定

using namespace cv; // 名前空間の指定

int main()
{
	// カメラからのビデオキャプチャを初期化する
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}

	//MOG前景処理するためのクラス
	cv::Ptr<BackgroundSubtractorMOG2> fgbg = createBackgroundSubtractorMOG2();


	// 何かキーが押下されるまで、ループをくり返す
	while (cvWaitKey(1) == -1)
	{
		// カメラから1フレーム取得する
		cv::Mat frame; //カメラフレーム
		if (!cap.read(frame))
			break;

		cv::Mat fgMaskMOG; //MOGを使った前景マスク(foreGround)

		//マスク処理（MOG処理を施した画像に変換）
		//太陽の位置の変化などゆっくりした動きに対して対応するため
		fgbg->apply(frame, fgMaskMOG);

		//二値画像に変換
		cv::Mat binImage;
		cv::threshold(fgMaskMOG, binImage, 0, 255.0, CV_THRESH_OTSU);
		int allPixelsNum = binImage.rows * binImage.cols; //全体の数
		int nonZero = cv::countNonZero(binImage); //白の数

		//全体の中の動体の割合
		double rate = (double)(cv::countNonZero(binImage)) / (double)allPixelsNum;

		//動体が通っているか
		if (rate > /*detectRate =*/ 0.2) {
			time_t timer = time(NULL);
			struct tm *pnow = new struct tm();
			localtime_s(pnow, &timer);
			printf("通った！ 現在時刻：%d : %d : %d\n", pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
			
		}

		// ウィンドウに画像を表示する
		cv::imshow("frame", binImage);

		//		printf("nonZero = %d allImagePixels = %d\n", &nonZero, &allPixelsNum);
		//		printf("row = %d cols = %d\n", &binImage.rows, &binImage.cols);
		//		printf("rate = %f allImagePixels = %d\n", &rate, &allPixelsNum);
	}

	cap.release();
	cv::destroyAllWindows();

	//ウインドウの表示時間(0はキーが押されるまで閉じない)
	cv::waitKey(0);

	return 0;
}
