#include "pch.h"
#include "ImageProcess.h"

Mat ImageProcess::RGB_2_Gray(Mat src)
{
    Mat result;
    cvtColor(src, result, cv::COLOR_RGB2GRAY);
    return result;
}

bool CheckSrcIsNull(Mat src)
{
	return src.empty() ? true : false;
}

void ImageProcess::Show_Histogram(Mat src)
{
	Mat image_gray, hist;   //��������ͼ�񣬻Ҷ�ͼ��, ֱ��ͼ
		
	if (src.empty())
	{
		MessageBoxA(NULL, "���ȵ���ͼƬ!!!", "����", MB_OK);
		return;
	}

	cvtColor(src, image_gray, COLOR_BGR2GRAY);  //�ҶȻ�
	//imshow("image_gray", image_gray);   //��ʾ�Ҷ�ͼ��

	//��ȡͼ��ֱ��ͼ
	int histsize = 256;
	float ranges[] = { 0,256 };
	const float* histRanges = { ranges };
	calcHist(&image_gray, 1, 0, Mat(), hist, 1, &histsize, &histRanges, true, false);

	//����ֱ��ͼ��ʾͼ��
	int hist_h = 300;//ֱ��ͼ��ͼ��ĸ�
	int hist_w = 512; //ֱ��ͼ��ͼ��Ŀ�
	int bin_w = hist_w / histsize;//ֱ��ͼ�ĵȼ�
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));//����ֱ��ͼ��ʾ��ͼ��

	//���Ʋ���ʾֱ��ͼ
	normalize(hist, hist, 0, hist_h, NORM_MINMAX, -1, Mat());//��һ��ֱ��ͼ
	for (int i = 1; i < histsize; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
	}

	imshow("histImage", histImage);
	waitKey(0);  //��ͣ������ͼ����ʾ���ȴ���������

}

Mat ImageProcess::EqualizeHist(Mat src)
{
	Mat image_gray, image_enhanced;		//����Ҷ�ͼ��ֱ��ͼ
	if (src.empty())
	{
		MessageBoxA(NULL, "���ȵ���ͼƬ!!!", "����", MB_OK);
		return Mat();
	}

	cvtColor(src, image_gray, COLOR_BGR2GRAY);	//�ҶȻ�
	//imshow("image_gray", image_gray);	

	equalizeHist(image_gray, image_enhanced);
	//imshow("image_enhanced", image_enhanced);
	return image_enhanced;
}

Mat ImageProcess::GrayScaleLinearTransform(Mat inputImage, double alpha, double beta)
{

	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	//���Ա任
	Mat transformImage = Mat::zeros(grayImage.size(), CV_8U);
	for (int y = 0; y < grayImage.rows; y++)
	{
		for (int x = 0; x < grayImage.cols; x++)
		{
			int originPixelValue = static_cast<int>(grayImage.at<uchar>(y, x));
			int newPixelValue = cv::saturate_cast<uchar>(alpha * originPixelValue + beta);
			transformImage.at<uchar>(y, x) = newPixelValue;
		}
	}

	return transformImage;
}

Mat ImageProcess::GrayLogTransform(Mat inputImage, double c)
{

	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	//�����任 new_image = c * log(1 + origin)
	Mat newTransformImage = Mat::zeros(grayImage.size(),CV_8U);
	for (int y = 0; y < inputImage.rows; y++)
	{
		for (int x = 0; x < inputImage.cols; x++)
		{
			int originalPixelValue = static_cast<int>(grayImage.at<uchar>(y, x));
			int newPixelValue = saturate_cast<uchar>(c * std::log(1 + originalPixelValue));
			newTransformImage.at<uchar>(y, x) = newPixelValue;
		}
	}
	return newTransformImage;
}

Mat ImageProcess::GrayScaleGamaTransform(Mat inputImage, double gama)
{

	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	//�Ҷ�٤��任 new_image = 255 * (origin / 255) ^ gama
	Mat newTransformImage = Mat::zeros(grayImage.size(), CV_8U);
	for (int y = 0; y < inputImage.rows; y++)
	{
		for (int x = 0; x < inputImage.cols; x++)
		{
			int originalPixelValue = static_cast<int>(grayImage.at<uchar>(y, x));
			int newPixelValue = saturate_cast<uchar>(255 * std::pow(originalPixelValue / 255, gama));
			newTransformImage.at<uchar>(y, x) = newPixelValue;
		}
	}
	return newTransformImage;
}

Mat ImageProcess::ImageErosion(Mat inputImage, int erosionSize)
{
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
	Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1),cv::Point(erosionSize, erosionSize));

	// ���и�ʴ����
	cv::Mat erodedImage;
	cv::erode(grayImage, erodedImage, element);

	return erodedImage;
}

Mat ImageProcess::ImageDilation(Mat inputImage, int dilationSize)
{
	// ��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	// �������ͺˣ��ṹԪ�أ�
	Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1),
		cv::Point(dilationSize, dilationSize));

	// �������Ͳ���
	Mat dilatedImage;
	dilate(grayImage, dilatedImage, element);

	return dilatedImage;
}

Mat ImageProcess::ImageThresholdSegmentation(Mat inputImage, int thresholdSize)
{
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	Mat segmentImage;
	threshold(grayImage, segmentImage, thresholdSize, 255, cv::THRESH_BINARY);
	return segmentImage;
}

Mat ImageProcess::ImageAdaptiveThresholdSegmentation(Mat inputImage, int blockSize, int subtractValue)
{
	// ��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	// ��������Ӧ��ֵ�ָ�
	Mat segmentedAdaptive;
	adaptiveThreshold(grayImage, segmentedAdaptive, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
		cv::THRESH_BINARY, blockSize, subtractValue);

	return segmentedAdaptive;
}

Mat ImageProcess::RegionGrowingSegmentation(Mat inputImage, int seedX, int seedY, int thresholdSize)
{
	// ��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	// ��ʼ���ָ�ͼ��
	Mat segmentedImage = cv::Mat::zeros(grayImage.size(), CV_8U);

	// ��ȡ���ӵ�ĳ�ʼ�Ҷ�ֵ
	int seedValue = static_cast<int>(grayImage.at<uchar>(seedY, seedX));

	// �����������ڱ�����������������
	std::queue<std::pair<int, int>> pixelQueue;
	pixelQueue.push(std::make_pair(seedX, seedY));

	// ��ʼ��������
	while (!pixelQueue.empty()) {
		int x = pixelQueue.front().first;
		int y = pixelQueue.front().second;
		pixelQueue.pop();

		// ��鵱ǰ�����Ƿ��ѱ������
		if (segmentedImage.at<uchar>(y, x) == 0) {
			// ���㵱ǰ���������ӵ�ĻҶȲ�
			int pixelValue = static_cast<int>(grayImage.at<uchar>(y, x));
			int diff = std::abs(seedValue - pixelValue);

			// ����ҶȲ�С����ֵ�������ָ�����
			if (diff <= thresholdSize) {
				segmentedImage.at<uchar>(y, x) = 255;

				// ����ǰ���ص�����������
				for (int dy = -1; dy <= 1; ++dy) {
					for (int dx = -1; dx <= 1; ++dx) {
						int newX = x + dx;
						int newY = y + dy;
						if (newX >= 0 && newX < grayImage.cols && newY >= 0 && newY < grayImage.rows) {
							pixelQueue.push(std::make_pair(newX, newY));
						}
					}
				}
			}
		}
	}

	return segmentedImage;
}

Mat ImageProcess::SobelEdgeDetection(Mat inputImage, int kernelSize)
{
	Mat grayImage;
	cvtColor(inputImage, grayImage, COLOR_BGR2GRAY);

	//����Sobel��Ե���
	Mat gradientX, gradientY;
	Sobel(grayImage, gradientX, CV_16S, 1, 0, kernelSize);
	Sobel(grayImage, gradientY, CV_16S, 0, 1, kernelSize);

	//�����ݶȷ�ֵ
	Mat absGradientX, absGradientY;
	convertScaleAbs(gradientX, absGradientX);
	convertScaleAbs(gradientX, absGradientY);

	//�ϲ�x��y���ݶȷ�ֵ
	Mat gradientImage;
	addWeighted(absGradientX, 0.5, absGradientY, 0.5, 0, gradientImage);


	return gradientImage;
}

Mat ImageProcess::CannyEdgeDetection(Mat inputImage, double threshold1, double threshold2)
{
	// ��RGBͼ��ת��Ϊ�Ҷ�ͼ��
	Mat grayImage;
	cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

	// ����Canny��Ե���
	Mat edgeImage;
	Canny(grayImage, edgeImage, threshold1, threshold2);

	return edgeImage;
}

Mat ImageProcess::MeanFilter(Mat inputImage, int kernelSize)
{
	Mat filterImage;
	blur(inputImage, filterImage, cv::Size(kernelSize, kernelSize));
	return filterImage;
}

Mat ImageProcess::MedianFilter(Mat inputImage, int kernelSize)
{
	Mat filterImage;
	medianBlur(inputImage, filterImage, kernelSize);
	return filterImage;
}

Mat ImageProcess::GaussianFilter(Mat inputImage, int kernelSize, double sigma)
{
	Mat filterImage;
	GaussianBlur(inputImage, filterImage, cv::Size(kernelSize, kernelSize), sigma);
	return filterImage;
}

