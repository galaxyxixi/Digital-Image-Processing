//
// Created by apple on 2020/6/12.
//
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

double generateGaussianNoise(double mu, double sigma);
Mat addGaussianNoise(Mat &srcImag);
Mat addSalt(const Mat srcImage, float ratio);
Mat addPepper(const Mat srcImage, float ratio);
vector<vector<double> > GenerateArv(int size);
Mat Smooth(Mat &srcImg, vector<vector<double> > gauss);
Mat Geometric(Mat srcImg, int n);
Mat Harmonic(Mat srcImg, int n);
Mat ContraHarmonic(Mat srcImg, int n, int p);
Mat Median(Mat srcImg, int n);
Mat Arv_Adaptive(Mat srcImg, int n, int Sn);
Mat Mid_Adaptive(Mat srcImg, int size);
