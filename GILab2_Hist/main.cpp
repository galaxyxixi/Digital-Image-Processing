#include<opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include "Hist.h"
using namespace cv;
using namespace std;

int main()
{
    Mat srcImg, dstImg, hist;
    printf("请输入实验名：1 计算归一化直方图；2 灰度图像的均衡化处理；3 彩色图像的均衡化处理");
    int exam;
    cin >> exam;
    switch(exam){
        case 1:
            srcImg = imread("./castle.jpg",0);
            dstImg = ShowHist(srcImg);
            break;
        case 2:
            srcImg = imread("./castle.jpg",0);
            dstImg = EqualizationGray(srcImg);
            //hist = ShowHist(srcImg);
            //imshow("hist", hist);
            break;
        case 3:
            srcImg = imread("./castle.jpg",1);
            dstImg = EqualizationRGB(srcImg);
            break;
        default:
            break;
    }
    imshow("srcImg", srcImg);
    imshow("dstImg", dstImg);
    waitKey();
    return 0;
}

