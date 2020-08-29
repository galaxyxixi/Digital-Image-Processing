//
// Created by apple on 2020/6/7.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

void Normalize(Mat &srcImg, int maxnum, int row, int col);
vector<vector<double> > GenerateArv(int size);
Mat avrSmooth(Mat &srcImg,int n);
void GenerateGaussian(vector<vector<double> > &gauss, int size);

Mat Smooth_Gray(Mat &srcImg, vector<vector<double> > gauss);
Mat Add(Mat srcImg, Mat edge, int k);

Mat Smooth_Color(Mat &srcImg, vector<vector<double> > gauss, int flag);
Mat Add_Color(Mat srcImg, Mat mask, int k);
