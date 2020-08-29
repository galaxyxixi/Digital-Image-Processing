#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "freqFilter.h"

using namespace std;
using namespace cv;

#define exam1 1
#define exam2 0
#define exam3 1

int main() {
    Mat srcImg;
    srcImg = imread("castle.jpg", 0);
    int row = srcImg.rows;
    int col = srcImg.cols;
    imshow("src",srcImg);
    copyMakeBorder(srcImg, srcImg,0,row,0,col,BORDER_CONSTANT);

#if exam1 > 0
    /***************傅里叶变换*************/
    srcImg.convertTo(srcImg,CV_64FC1);//转成浮点数据类型
    Mat dftDst(srcImg.size(),CV_64FC1); //预设dft的输出结果矩阵
    dft(srcImg, dftDst,DFT_COMPLEX_OUTPUT,0);//离散傅立叶变换
    dftshift(dftDst);
    displayDftSpectrum(dftDst,"dft",false);//显示傅里叶频谱图

    /***************傅里叶逆变换*************/
    Mat idftDst;
    dft(dftDst, idftDst, DFT_INVERSE);
    idftDst = idftDst(Rect(0, 0, col, row));
    displayDftSpectrum(idftDst,"idft",true);//显示傅里叶频谱图
#endif

#if exam2 > 0
    /***************理想低/高同滤波*************/
    Mat ideal(dftDst.size(),CV_64FC2,Scalar::all(0));
    IdealFilter(ideal,240,false);
    Mat ILPF = dftDst.mul(ideal);//复数频谱图与滤波器相乘，实现频域滤波
    srcCentralized(ILPF);
    Mat IdealPass;
    dft(ILPF, IdealPass, DFT_INVERSE);
    IdealPass = IdealPass(Rect(0, 0, col, row));
    displayDftSpectrum(IdealPass,"IdealFilter",true);//显示傅里叶频谱图
#endif

#if exam3 > 0
    /***************buterworth低/高同滤波*************/
    Mat butterworthLow(dftDst.size(),CV_64FC2,Scalar::all(0));
    ButterWorthLowerFilter(butterworthLow,240,2);
    Mat BLPF = dftDst.mul(butterworthLow);//复数频谱图与滤波器相乘，实现频域滤波
    srcCentralized(BLPF);
    Mat ButterworthLowPass;
    dft(BLPF, ButterworthLowPass, DFT_INVERSE);
    ButterworthLowPass = ButterworthLowPass(Rect(0, 0, col, row));
    displayDftSpectrum(ButterworthLowPass,"ButterworthLowFilter",true);//显示傅里叶频谱图

    Mat butterworthHigh(dftDst.size(),CV_64FC2,Scalar::all(0));
    ButterWorthHigherFilter(butterworthHigh,240,2);
    Mat BHPF = dftDst.mul(butterworthHigh);//复数频谱图与滤波器相乘，实现频域滤波
    srcCentralized(BHPF);
    Mat ButterworthHighPass;
    dft(BHPF, ButterworthHighPass, DFT_INVERSE);
    ButterworthHighPass = ButterworthHighPass(Rect(0, 0, col, row));
    displayDftSpectrum(ButterworthHighPass,"ButterworthHighFilter",true);//显示傅里叶频谱图
#endif

    waitKey();
    return 0;
}
