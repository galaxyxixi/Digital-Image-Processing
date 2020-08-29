//
// Created by apple on 2020/6/7.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "filtering.h"
using namespace cv;
using namespace std;

#define PI 3.1415926

//均值平滑滤波
//
vector<vector<double> > GenerateArv(int size){
    vector<vector<double> > mask(size, vector<double>(size));
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            mask[i][j] = 1.0/(size*size);
        }
    }
    return mask;
}

Mat avrSmooth(Mat &srcImg,int n){//n越大表示平滑程度越高
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    if(n % 2 == 0) n += 1; //算子模板的边长应为奇数

    int x = (n-1)/2;
    int oper[n][n];         //平滑算子
    for(int i = 0; i<n; i++){
        for(int j = 0; j<n; j++){
            oper[i][j] = 1;
        }
    }

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            int temp = 0;//用来计算新的中心像素点值
            for(int k = 0;k < n;k++){
                for(int l = 0;l < n;l++){
                    //当算子位于边缘区域时，排除图像中不存在的点
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    temp += (oper[k][l]) * srcImg.at<uchar>(i-x+k,j-x+l);
                }
            }
            temp/=(n*n);
            //将计算结果赋给中心像素点
            dstImg.at<uchar>(i,j) = temp;

        }
    }
    return dstImg;
}

//高斯滤波

//生成高斯模板
void GenerateGaussian(vector<vector<double> > &gauss, int size) {
    cout << "请输入高斯半径：" << endl;
    double sigma;
    cin >> sigma;
    int center = size / 2; //模板的中心位置，也就是坐标原点
    double sum = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            gauss[i][j] = (1 / (2 * PI * sigma * sigma)) *
                         exp(-((i - center) * (i - center) + (j - center) * (j - center)) / (2 * sigma * sigma));
            sum += gauss[i][j];
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            gauss[i][j] /= sum;
        }
    }

}

Mat Smooth_Gray(Mat &srcImg, vector<vector<double> > gauss){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int n = gauss.size();
    int x = (n-1)/2;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            double sum = 0;
            for(int k = 0;k < n;k++){
                vector<double> temp = gauss[k];
                for(int l = 0;l < n;l++){
                    //当算子位于边缘区域时，排除图像中不存在的点
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    sum += temp[l] * srcImg.at<uchar>(i-x+k,j-x+l);
                }
            }
            //将计算结果赋给中心像素点,另外防止像素值溢出
            if(sum < 0) sum = 0;
            if(sum > 255) sum = 255;
            dstImg.at<uchar>(i,j) = sum;
        }
    }
    return dstImg;
}

Mat Add(Mat srcImg, Mat mask, int k){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            dstImg.at<uchar>(i,j) = saturate_cast<uchar>(srcImg.at<uchar>(i,j) + mask.at<uchar>(i,j) * k)*0.9;
        }
    }
    return dstImg;
}

