//
// Created by apple on 2020/5/29.
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//检查图像是否读取成功
int check(Mat srcImg){
    if (!srcImg.data) {
        cout << "读入图片失败" << endl;
        return -1;
    }
    return 0;
}
//case 1:


//case 2: 二值变换
Mat ToBinary(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    printf("请输入二值变换的阈值：");
    int threshold;
    cin >> threshold;
    for(int i = 0; i<row; i++){
        uchar *srcData = srcImg.ptr<uchar>(i);
        for(int j = 0; j<col; j++){
            //超过阈值即为255，否则为0
            dstImg.at<uchar>(i, j) = (srcData[j] >= threshold) ? 255 : 0;
        }
    }
    return dstImg;
}

//case 3: 对数变换
Mat Logarithmic(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    double gray = 0;
    int coef;
    printf("请输入对数变换系数：");
    cin >> coef;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            gray = (double)srcImg.at<uchar>(i, j);
            gray = coef * log((double)(1 + gray));
            dstImg.at<uchar>(i, j) = saturate_cast<uchar>(gray);
        }
    }
    normalize(dstImg, dstImg, 0, 255, cv::NORM_MINMAX);
    return dstImg;
}

//case 4 伽马变换
Mat Gamma(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    unsigned char LUT[256];
    printf("请输入gamma变换指数：");
    float r;
    cin >> r;
    for (int i = 0; i < 256; i++){
        LUT[i] = saturate_cast<uchar>(pow((double)(i / 255.0), r)*255.0); //Gamma变换定义
    }
    for(int i = 0; i<row; i++){
        uchar *srcData = srcImg.ptr<uchar>(i);
        for(int j = 0; j<col; j++){
            dstImg.at<uchar>(i, j) = LUT[srcData[j]];
        }

    }
    return dstImg;
}

//case 5: 补色变换
Mat Complementary(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    for (int i = 0; i < row;++i) {
        for (int j = 0; j < col; ++j) {
            Vec3d vec = srcImg.at<Vec3b>(i, j);
            int b = vec[0];//blue
            int g = srcImg.at<Vec3b>(i, j)[1];//green
            int r = srcImg.at<Vec3b>(i, j)[2];//red
            int maxrgb = max(max(r, g), b);
            int minrgb = min(min(r, g), b);
            dstImg.at<Vec3b>(i, j)[0] = maxrgb + minrgb - b;
            dstImg.at<Vec3b>(i, j)[1] = maxrgb + minrgb - g;
            dstImg.at<Vec3b>(i, j)[2] = maxrgb + minrgb - r;

        }
    }
    return dstImg;
}