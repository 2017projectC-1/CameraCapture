#include "stdafx.h"
#include "time.h"

#include <opencv2/opencv.hpp> // �C���N���[�h�t�@�C���w��
//#include <opencv2/videoio/>
//#include <opencv2/opencv_lib.hpp> // �ÓI�����N���C�u�����̎w��

using namespace cv; // ���O��Ԃ̎w��

int main()
{
	// �J��������̃r�f�I�L���v�`��������������
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}

	//MOG�O�i�������邽�߂̃N���X
	cv::Ptr<BackgroundSubtractorMOG2> fgbg = createBackgroundSubtractorMOG2();


	// �����L�[�����������܂ŁA���[�v������Ԃ�
	while (cvWaitKey(1) == -1)
	{
		// �J��������1�t���[���擾����
		cv::Mat frame; //�J�����t���[��
		if (!cap.read(frame))
			break;

		cv::Mat fgMaskMOG; //MOG���g�����O�i�}�X�N(foreGround)

		//�}�X�N�����iMOG�������{�����摜�ɕϊ��j
		//���z�̈ʒu�̕ω��Ȃǂ�����肵�������ɑ΂��đΉ����邽��
		fgbg->apply(frame, fgMaskMOG);

		//��l�摜�ɕϊ�
		cv::Mat binImage;
		cv::threshold(fgMaskMOG, binImage, 0, 255.0, CV_THRESH_OTSU);
		int allPixelsNum = binImage.rows * binImage.cols; //�S�̂̐�
		int nonZero = cv::countNonZero(binImage); //���̐�

		//�S�̂̒��̓��̂̊���
		double rate = (double)(cv::countNonZero(binImage)) / (double)allPixelsNum;

		//���̂��ʂ��Ă��邩
		if (rate > /*detectRate =*/ 0.2) {
			time_t timer = time(NULL);
			struct tm *pnow = new struct tm();
			localtime_s(pnow, &timer);
			printf("�ʂ����I ���ݎ����F%d : %d : %d\n", pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
			
		}

		// �E�B���h�E�ɉ摜��\������
		cv::imshow("frame", binImage);

		//		printf("nonZero = %d allImagePixels = %d\n", &nonZero, &allPixelsNum);
		//		printf("row = %d cols = %d\n", &binImage.rows, &binImage.cols);
		//		printf("rate = %f allImagePixels = %d\n", &rate, &allPixelsNum);
	}

	cap.release();
	cv::destroyAllWindows();

	//�E�C���h�E�̕\������(0�̓L�[���������܂ŕ��Ȃ�)
	cv::waitKey(0);

	return 0;
}
