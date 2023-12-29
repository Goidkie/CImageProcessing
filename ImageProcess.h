#pragma once
class ImageProcess
{

public:
	void Show_Histogram(Mat src);												//��ʾ�Ҷ�ֱ��ͼ


	/**
		* �Ҷȴ���
	*/
	Mat RGB_2_Gray(Mat src);													//ͼƬת�Ҷ�
	Mat EqualizeHist(Mat src);													//ֱ��ͼ���⻯
	Mat GrayScaleLinearTransform(Mat InputImage, double alpha, double beta);	//���ԻҶȱ任
	Mat GrayLogTransform(Mat inputImage, double c);								//�Ҷȶ����任
	Mat GrayScaleGamaTransform(Mat inputImage, double gama);					//�Ҷ� �� �任

	/**
		* ��̬ѧ����
	*/
	Mat ImageErosion(Mat inputImage, int erosionSize);			//��ʴ
	Mat ImageDilation(Mat inputImage, int dilationSize);		//����

	/**
		* ͼ����
	*/
	Mat ImageThresholdSegmentation(Mat inputImage, int thresholdSize);							//�̶���ֵ
	Mat ImageAdaptiveThresholdSegmentation(Mat inputImage, int blockSize, int subtractValue);	//����Ӧ��ֵ
	Mat RegionGrowingSegmentation(Mat inputImage, int seedX, int seedY, int thresholdSize);		//���������ָ�
	Mat SobelEdgeDetection(Mat inputImage, int kernelSize);										//Sobel�㷨
	Mat CannyEdgeDetection(Mat inputImage, double threshold1, double threshold2);				//Canny�㷨

	/**
		* ͼ���˲�
	*/
	Mat MeanFilter(Mat inputImage, int kernelSize);												//��ֵ�˲�
	Mat MedianFilter(Mat inputImage, int kernelSize);											//��ֵ�˲�
	Mat GaussianFilter(Mat inputImage, int kernelSize, double sigma);							//��˹�˲�
};

