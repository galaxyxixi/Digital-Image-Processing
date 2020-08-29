//
// Created by apple on 2020/5/29.
//
using namespace std;
using namespace cv;

#ifndef GRAY_GRAY_H
#define GRAY_GRAY_H

int check(Mat srcImg);

//case 2 二值变换
Mat ToBinary(Mat srcImg);

//case 3 对数变换
Mat Logarithmic(Mat srcImg);

//case 4 伽马变换
Mat Gamma(Mat srcImg);

//case 5 补色变换
Mat Complementary(Mat srcImg);

#endif //GRAY_GRAY_H
