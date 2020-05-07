#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace cv;
using namespace std;

int LevelCompare(float Angle)
{
	int level = 0;
	switch ((int)(Angle / 45))
	{
	case 0:
		level = 1;
		break;
	case 1:
		level = 2;
		break;
	case 2:
		level = 3;
		break;
	case 3:
		level = 4;
		break;
	case 4:
		level = 5;
		break;
	case 5:
		level = 6;
		break;
	case 6:
		level = 7;
		break;
	case 7:
		level = 8;
		break;
	default:
		level = 0;
		break;
	}
	return level;
}

int main()
{
	cv::Mat refMat = imread("C:\\picture\\hogTemplate.jpg", 1);
	cv::Mat imgMat1 = imread("C:\\picture\\img1.jpg", 1);
	cv::Mat imgMat2 = imread("C:\\picture\\img2.jpg", 1);
	int cellSize = 16;
	int nX = refMat.cols / cellSize;
	int nY = refMat.rows / cellSize;
	int bins = nX * nY * 8;

	float* ref_hist = new float[bins];               //创建直方图数组
	memset(ref_hist, 0, sizeof(float) * bins);
	float* ref_hist_img1 = new float[bins];          //创建直方图数组
	memset(ref_hist_img1, 0, sizeof(float) * bins);
	float* ref_hist_img2 = new float[bins];          //创建直方图数组
	memset(ref_hist_img2, 0, sizeof(float) * bins);

	Mat gx, gy;                                      //把每一幅图的角度梯度算出
	Mat mag, angle;
	Sobel(refMat, gx, CV_32F, 1, 0, 1);
	Sobel(refMat, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);
	Mat gxImg1, gyImg1;
	Mat magImg1, angleImg1;
	Sobel(imgMat1, gxImg1, CV_32F, 1, 0, 1);
	Sobel(imgMat1, gyImg1, CV_32F, 0, 1, 1);
	cartToPolar(gxImg1, gyImg1, magImg1, angleImg1, true);
	Mat gxImg2, gyImg2;
	Mat magImg2, angleImg2;
	Sobel(imgMat2, gxImg2, CV_32F, 1, 0, 1);
	Sobel(imgMat2, gyImg2, CV_32F, 0, 1, 1);
	cartToPolar(gxImg2, gyImg2, magImg2, angleImg2, true);

	int binnum = 0;
	for (int ny = 0; ny < nY; ny++)
	{
		for (int nx = 0; nx < nX; nx++)
		{               //计算每一个cell里面的直方图
			for (int cy = ny * cellSize; cy < (ny + 1) * cellSize; cy++)
			{
				for (int cx = nx * cellSize; cx < (nx + 1) * cellSize; cx++)
				{
					int dimension = LevelCompare(angle.at<float>(cy, cx));
					float magnitude = mag.at<float>(cy, cx);
					ref_hist[dimension + binnum * 8] += magnitude;

					int dimensionImg1 = LevelCompare(angleImg1.at<float>(cy, cx));
					float magnitudeImg1 = magImg1.at<float>(cy, cx);
					ref_hist_img1[dimensionImg1 + binnum * 8] += magnitudeImg1;

					int dimensionImg2 = LevelCompare(angleImg2.at<float>(cy, cx));
					float magnitudeImg2 = magImg2.at<float>(cy, cx);
					ref_hist_img2[dimensionImg2 + binnum * 8] += magnitudeImg2;
				}
			}
			binnum++;
		}
	}
	float img1Num = 0;
	float img2Num = 0;
	int img1Fangcha = 0;
	int img2Fangcha = 0;
	for (int i = 0; i < bins; i++)
	{
		img1Num += ((ref_hist[i] - ref_hist_img1[i]) * (ref_hist[i] - ref_hist_img1[i]));
		img2Num += ((ref_hist[i] - ref_hist_img2[i]) * (ref_hist[i] - ref_hist_img2[i]));
	}
	img1Fangcha = sqrt(img1Num);
	img2Fangcha = sqrt(img2Num);
	cout << "img1: " << img1Fangcha << endl;
	cout << "img2: " << img2Fangcha << endl;
	if (img1Fangcha > img2Fangcha)
	{
		cout << "图片二更相近" << endl;
	}
	else
		cout << "图片一更相近" << endl;
	delete[]ref_hist;
	delete[]ref_hist_img1;
	delete[]ref_hist_img2;
	return 0;
}