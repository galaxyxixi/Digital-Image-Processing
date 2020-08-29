//
// Created by apple on 2020/6/3.
//

#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat ShowHist(Mat srcImg){
    Mat dstImg = Mat::zeros(Size(256, 256), CV_8UC3);
    int row = srcImg.rows;
    int col = srcImg.cols;
    int nums = row * col;
    int g[256] = {0};
    for(int i = 0; i<row; i++){
        uchar *srcData = srcImg.ptr<uchar>(i);
        for(int j = 0; j<col; j++){
            g[srcData[j]]++;
        }
    }
    int maxg = 0;
    for(int i = 0; i<256; i++) maxg = max(maxg, g[i]);
    /*
     * 灰度级为16级
    int h[16] = {0};
    for(int i = 0; i<16; i++){
        int sum = 0;
        for(int j = 0; j<16; j++){
            sum += g[i*16 + j];
        }
        h[i] = sum;
        maxg = max(maxg,h[i]);
    }
    */
    for (int i = 0; i < 256; i++)
    {
        int value = g[i] * 256 / maxg;
        //注意：坐标原点在左上角
        line(dstImg,Point(i, dstImg.rows - 1),
                    Point(i, dstImg.rows - 1 - value),
                   Scalar(255, 255, 255));

        //rectangle(dstImg, Point(i*30,dstImg.rows - 1), Point(i*30+30, dstImg.rows - 1 - value), Scalar(255, 255, 255));
    }
    return dstImg;
}

//灰度图像的均衡化处理
Mat EqualizationGray(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int nums = row * col;
    int g[256] = {0};
    for(int i = 0; i<row; i++){
        uchar *srcData = srcImg.ptr<uchar>(i);
        for(int j = 0; j<col; j++){
            g[srcData[j]]++;
        }
    }
    int sum = 0;
    for(int i = 0; i<256; i++){
        sum += g[i];
        g[i] = sum * 255 / nums;
    }

    for(int i = 0; i<row; i++) {
        uchar *srcData = srcImg.ptr<uchar>(i);
        for (int j = 0; j < col; j++) {
            dstImg.at<uchar>(i, j) = g[srcData[j]];
        }
    }
    return dstImg;

}
//彩色图像直方图均衡处理
Mat EqualizationRGB(Mat srcImg){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int nums = row * col;
    int r[256] = {0}, g[256] = {0}, b[256] = {0};
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            Vec3b cur = srcImg.at<Vec3b>(i, j);
            int b_ = cur[0];//blue
            int g_= srcImg.at<Vec3b>(i, j)[1];//green
            int r_ = srcImg.at<Vec3b>(i, j)[2];//red
            b[b_]++;
            g[g_]++;
            r[r_]++;
        }
    }
    int sum_b = 0, sum_g = 0, sum_r = 0;
    for(int i = 0; i<256; i++){
        sum_b += b[i];
        sum_g += g[i];
        sum_r += r[i];
        b[i] = sum_b * 255 / nums;
        g[i] = sum_g * 255 / nums;
        r[i] = sum_r * 255 / nums;
    }

    for(int i = 0; i<row; i++) {
        for (int j = 0; j < col; j++) {
            int b_ = srcImg.at<Vec3b>(i, j)[0];//blue
            int g_= srcImg.at<Vec3b>(i, j)[1];//green
            int r_ = srcImg.at<Vec3b>(i, j)[2];//red
            dstImg.at<Vec3b>(i, j)[0] = b[b_];
            dstImg.at<Vec3b>(i, j)[1] = g[g_];
            dstImg.at<Vec3b>(i, j)[2] = r[r_];
        }
    }
    return dstImg;
}
