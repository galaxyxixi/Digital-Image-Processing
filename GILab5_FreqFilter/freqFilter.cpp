//
// Created by apple on 2020/6/18.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define CV_MAT_ELEM2(src,dtype,y,x) \
        (dtype*)(src.data+src.step[0]*y+src.step[1]*x)

using namespace std;
using namespace cv;

void myMagnitude(Mat & complexImg,Mat & mI)
{
    Mat planes[2];
    split(complexImg,planes);
    magnitude(planes[0],planes[1],mI);
}

void dftshift(Mat& ds){
    int cx=ds.cols/2;//图像的中心点x 坐标
    int cy=ds.rows/2;//图像的中心点y 坐标
    Mat q0=ds(Rect(0,0,cx,cy));//左上
    Mat q1=ds(Rect(cx,0,cx,cy));//右上
    Mat q2=ds(Rect(0,cy,cx,cy));//左下
    Mat q3=ds(Rect(cx,cy,cx,cy));//右下
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void displayDftSpectrum(Mat&dftDst,String winName,bool inverseSpectrum){
    Mat magI;
    myMagnitude(dftDst,magI);
    if(!inverseSpectrum)//如果是正向傅里叶变换谱
    {
        magI+=Scalar::all(1);
        log(magI,magI);
    }
    normalize(magI,magI,0,1,NORM_MINMAX);
    imshow(winName,magI);
}

void srcCentralized(Mat& src){
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            float* mv = CV_MAT_ELEM2(src,float,i,j);
            if((i+j)%2 != 0)
                mv[0] = -mv[0];//如果i+j为奇数，该像素值取负值
        }
    }
}


void IdealFilter(Mat &filter, int d0, bool isLowPassFilter){
    int cx=filter.cols/2,cy=filter.rows/2;
    int D=d0*d0;

    for(int i=0;i<filter.rows;i++){
        Vec2d* pf=filter.ptr<Vec2d>(i);
        for(int j=0;j<filter.cols;j++){
            int d=(j-cx)*(j-cx)+(i-cy)*(i-cy);
            if(d < D)
            {
                pf[j][0]=1;
                pf[j][1]=pf[j][0];
            }
        }
    }
    if(!isLowPassFilter)
    {
        filter =Scalar::all(1)-filter;
    }
    Mat displayFilter;
    extractChannel(filter,displayFilter,0);
    imshow("filter image",displayFilter);
}

void ButterWorthLowerFilter(Mat &filter, int d0, int n){
    int cx=filter.cols/2,cy=filter.rows/2;
    for(int i=0;i<filter.rows;i++){
        Vec2d* pf=filter.ptr<Vec2d>(i);
        for(int j=0;j<filter.cols;j++){
            double d = sqrt((j-cx)*(j-cx)+(i-cy)*(i-cy));
            double h =pow(1.0/(1+(d/d0)), 2*n);
            pf[j][0] = h;
            pf[j][1] = pf[j][0];
        }
    }
    Mat displayFilter;
    extractChannel(filter,displayFilter,0);
    imshow("filterLow image",displayFilter);
}

void ButterWorthHigherFilter(Mat &filter, int d0, int n){
    int cx=filter.cols/2,cy=filter.rows/2;
    for(int i=0;i<filter.rows;i++){
        Vec2d* pf=filter.ptr<Vec2d>(i);
        for(int j=0;j<filter.cols;j++){
            double d = sqrt((j-cx)*(j-cx)+(i-cy)*(i-cy));
            double h =pow(1.0/(1+(d0/d)), 2*n);
            pf[j][0] = h;
            pf[j][1] = pf[j][0];
        }
    }
    Mat displayFilter;
    extractChannel(filter,displayFilter,0);
    imshow("filterHigh image",displayFilter);
}