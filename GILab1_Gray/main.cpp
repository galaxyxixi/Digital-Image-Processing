#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Gray.h"

using namespace std;
using namespace cv;

int main()
{
    Mat srcImg, dstImg;
    printf("请输入实验名：1 显示图像；2 二值化处理；3 对数变换；4 伽马变换；5 补色变换");
    int exam;
    cin >> exam;
    switch(exam){
        case 1:
            srcImg = imread("./castle.jpg",0);
            if(check(srcImg) < 0) return -1;
            imshow("srcImg", srcImg);
            waitKey();
        case 2:
            srcImg = imread("./castle.jpg",0);
            if(check(srcImg) < 0) return -1;
            dstImg = ToBinary(srcImg);
            break;
        case 3:
            srcImg = imread("./castle.jpg",0);
            if(check(srcImg) < 0) return -1;
            dstImg = Logarithmic(srcImg);
            break;
        case 4:
            srcImg = imread("./castle.jpg",0);
            if(check(srcImg) < 0) return -1;
            dstImg = Gamma(srcImg);
            break;
        case 5:
            srcImg = imread("./castle.jpg",1);
            if(check(srcImg) < 0) return -1;
            dstImg = Complementary(srcImg);
            break;
        default:
            break;
    }

    imshow("srcImg", srcImg);
    imshow("dstImg", dstImg);
    waitKey(0);
    return 0;
}
