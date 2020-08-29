//
// Created by apple on 2020/6/18.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void myMagnitude(Mat & complexImg,Mat & mI);
void dftshift(Mat& ds);
void displayDftSpectrum(Mat&dftDst,String winName,bool inverseSpectrum);
void srcCentralized(Mat& srcImg);
void IdealFilter(Mat&filter,int d0,bool isLowPassFilter);
void ButterWorthLowerFilter(Mat &filter, int d0, int n);
void ButterWorthHigherFilter(Mat &filter, int d0, int n);



