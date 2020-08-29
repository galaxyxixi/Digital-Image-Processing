//
// Created by apple on 2020/6/12.
//
#include <iostream>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

double generateGaussianNoise(double mu, double sigma) {
    //定义小值
    const double epsilon = numeric_limits<double>::min();
    static double z0, z1;
    static bool flag = false;
    flag = !flag;
    //flag为假构造高斯随机变量X
    if (!flag)
        return z1 * sigma + mu;
    double u1, u2;
    //构造随机变量
    do {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);
    //flag为真构造高斯随机变量
    z0 = sqrt(-2.0 * log(u1)) * cos(2 * CV_PI * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(2 * CV_PI * u2);
    return (int)(z0 * sigma + mu);
}


//为图像添加高斯噪声
Mat addGaussianNoise(Mat &srcImg)
{
    Mat dstImg = srcImg.clone();
    //Mat dstImg(srcImg.size(), srcImg.type());
    int channels = srcImg.channels();
    int rowsNumber = srcImg.rows;
    int colsNumber = srcImg.cols*channels;
    for (int i = 0; i < rowsNumber; i++)
    {
        for (int j = 0; j < colsNumber; j++)
        {
            //添加高斯噪声
            int val = dstImg.ptr<uchar>(i)[j] + generateGaussianNoise(0, 1) * 32;
            if (val < 0) val = 0;
            if (val>255)  val = 255;
            dstImg.ptr<uchar>(i)[j] = (uchar)val;
        }
    }
    return dstImg;
}


Mat addSalt(const Mat srcImage, float ratio) {
    Mat dstImage = srcImage.clone();
    int n = srcImage.rows * srcImage.cols * ratio;
    for (int k = 0; k < n; k++) {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1) {
            dstImage.at<uchar>(i, j) = 255;       //盐噪声
        } else {
            dstImage.at<Vec3b>(i, j)[0] = 255;
            dstImage.at<Vec3b>(i, j)[1] = 255;
            dstImage.at<Vec3b>(i, j)[2] = 255;
        }
    }
    return dstImage;
}

Mat addPepper(const Mat srcImage, float ratio){
    Mat dstImage = srcImage.clone();
    int n = srcImage.rows * srcImage.cols * ratio;
    for (int k = 0; k < n; k++) {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1) {
            dstImage.at<uchar>(i, j) = 0;     //椒噪声
        } else {
            dstImage.at<Vec3b>(i, j)[0] = 0;
            dstImage.at<Vec3b>(i, j)[1] = 0;
            dstImage.at<Vec3b>(i, j)[2] = 0;
        }
    }
    return dstImage;
}

//均值平滑滤波
vector<vector<double> > GenerateArv(int size){
    vector<vector<double> > mask(size, vector<double>(size));
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            mask[i][j] = 1.0/(size*size);
        }
    }
    return mask;
}

Mat Smooth(Mat &srcImg, vector<vector<double> > gauss){
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
//几何均值滤波
Mat Geometric(Mat srcImg, int n){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int x = (n-1)/2;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            double mult = 1.0;
            double num = 0;
            for(int k = 0; k<n; k++){
                for(int l = 0; l<n; l++){
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    uchar cur = srcImg.at<uchar>(i-x+k,j-x+l);
                    if(cur == 0) continue;
                    mult *= cur;
                    num++;
                }
            }
            auto temp = pow(mult, 1.0/num);
            if(temp > 255) temp = 255;
            if(temp < 0) temp = 0;
            dstImg.at<uchar>(i,j) = temp;
        }
    }
    return dstImg;
}
//谐波均值
Mat Harmonic(Mat srcImg, int n){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int x = (n-1)/2;
    int nums = n*n;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            double sum = 0;
            for(int k = 0; k<n; k++){
                for(int l = 0; l<n; l++){
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    uchar cur = srcImg.at<uchar>(i-x+k,j-x+l);
                    if(cur != 0) sum += 1.0/cur;
                }
            }
            if(sum == 0) continue;
            uchar data = nums/sum;
            if(data > 255) data = 255;
            if(data < 0) data = 0;
            dstImg.at<uchar>(i,j) = data;
        }
    }
    return dstImg;
}
//逆谐波均值
Mat ContraHarmonic(Mat srcImg, int n, int p){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int x = (n-1)/2;
    int nums = n*n;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            double sumUp = 0, sumDown = 0;
            for(int k = 0; k<n; k++){
                for(int l = 0; l<n; l++){
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    uchar cur = srcImg.at<uchar>(i-x+k,j-x+l);
                    sumDown += pow(cur, p);
                    sumUp += pow(cur, p+1);
                }
            }
            if(sumUp == 0 || sumDown == 0) continue;
            uchar data = sumUp/sumDown;
            if(data > 255) data = 255;
            if(data < 0) data = 0;
            dstImg.at<uchar>(i,j) = data;
        }
    }
    return dstImg;
}
//中值滤波
Mat Median(Mat srcImg, int n){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    int x = (n-1)/2;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            vector<int> vec;
            for(int k = 0; k<n; k++){
                for(int l = 0; l<n; l++){
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    vec.push_back(srcImg.at<uchar>(i-x+k,j-x+l));
                }
            }
            sort(vec.begin(), vec.end());
            uchar data = vec[(int)vec.size()/2];
            if(data > 255) data = 255;
            if(data < 0) data = 0;
            dstImg.at<uchar>(i,j) = data;
        }
    }
    return dstImg;
}
//自适应均值滤波
Mat Arv_Adaptive(Mat srcImg, int n, int Sn){
    Mat dstImg(srcImg.size(), srcImg.type()), meanImg(srcImg.size(), srcImg.type());
    vector<vector<double> > Arv;
    Arv = GenerateArv(n);
    meanImg = Smooth(srcImg, Arv);
    int row = dstImg.rows;
    int col = dstImg.cols;
    int x = (n-1)/2;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            int nums = 0;
            double sigma = 1;
            for(int k = 0; k<n; k++){
                for(int l = 0; l<n; l++){
                    if((i-x+k < 0) || (j-x+l < 0) || (i-x+k >= row) || (j-x+l >= col))
                        continue;
                    double sum = srcImg.at<uchar>(i-x+k,j-x+l) - meanImg.at<uchar>(i,j);
                    if(sum != 0){
                        sigma += pow(sum,2);
                        nums++;
                    }
                }
            }
            sigma /= nums; //方差
            uchar temp = srcImg.at<uchar>(i,j) - (Sn/sigma)*(srcImg.at<uchar>(i,j) - meanImg.at<uchar>(i,j));
            if(temp > 255) temp = 255;
            if(temp < 0) temp = 0;
            dstImg.at<uchar>(i,j) = temp;
        }
    }
    return dstImg;
}

Mat Mid_Adaptive(Mat srcImg, int size){
    Mat dstImg(srcImg.size(), srcImg.type());
    int row = dstImg.rows;
    int col = dstImg.cols;
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            int n = 3;
            bool unfinish = true;
            while(unfinish) {
                int x = (n-1)/2;
                int Zmin = 255, Zmax = 0;
                vector<int> vec;
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < n; l++) {
                        if ((i - x + k < 0) || (j - x + l < 0) || (i - x + k >= row) || (j - x + l >= col))
                            continue;
                        int temp = srcImg.at<uchar>(i - x + k, j - x + l);
                        vec.push_back(temp);
                        Zmin = Zmin < temp ? Zmin : temp;
                        Zmax = Zmax > temp ? Zmax : temp;
                    }
                }
                sort(vec.begin(), vec.end());
                int mid = vec[(int) vec.size() / 2];
                if (mid > Zmin && mid < Zmax) {
                    int cur = srcImg.at<uchar>(i, j);
                    if (cur > Zmin && cur < Zmax) dstImg.at<uchar>(i, j) = cur;
                    else dstImg.at<uchar>(i, j) = mid;
                    unfinish = false;
                } else {
                    n += 2;
                    if (n <= size) unfinish = true;
                    else {
                        dstImg.at<uchar>(i, j) = mid;
                        unfinish = false;
                    }
                }
            }
        }
    }
    return dstImg;
}
