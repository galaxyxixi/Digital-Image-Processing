#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "filtering.h"
using namespace cv;
using namespace std;




int main() {
    //laplacian算子：
    vector<vector<double> > laplacian = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};      //laplacian算子

    //Robert算子，四个方向：
    vector<vector<double> > Robert_horizontal = {{-1,-2,-1},{0,0,0},{1,2,1}};
    vector<vector<double> > Robert_vertical = {{-1,0,-1},{-2,0,2},{-1,0,1}};
    vector<vector<double> > Robert_diagonal = {{0,1,2},{-1,0,1},{-2,-1,0}};
    vector<vector<double> > Robert_anti_diagonal = {{-2,-1,0},{-1,0,1},{0,1,2}};

    //Sobel算子,两个方向
    vector<vector<double> > Sobel_diagonal = {{0,-1},{1,0}};
    vector<vector<double> > Sobel_anti_diagonal = {{-1,0},{0,1}};

    Mat srcImg, dstImg, hist;
    int size;
    printf("请输入实验名：1 利用均值模板平滑灰度图像；"
           "2 利用高斯模板平滑灰度图像；"
           "3 利用 Laplacian、Robert、Sobel 模板锐化灰度图像；"
           "4 利用高提升滤波算法增强灰度图像 "
           "5 利用均值模板平滑彩色图像; "
           "6 利用高斯模板平滑彩色图像; "
           "7 利用 Laplacian、Robert、Sobel 模板锐化灰度图像");
    int exam;
    cin >> exam;
    switch(exam){
        case 1: {
            srcImg = imread("./castle.jpg", 0);
            cout << "请输入均值模版尺寸(3/5/9)" << endl;
            cin >> size;
            vector<vector<double> > Arv;
            Arv = GenerateArv(size);
            dstImg = Smooth_Gray(srcImg, Arv);
            break;
        }
        case 2: {
            srcImg = imread("./castle.jpg", 0);
            cout << "请输入高斯模版尺寸(3/5/9)" << endl;
            cin >> size;
            vector<vector<double> > Gaussian(size, vector<double>(size));
            GenerateGaussian(Gaussian, size);   //生成高斯模版
            dstImg = Smooth_Gray(srcImg, Gaussian);  //利用高斯模版进行平滑
            break;
        }
        case 3:{
            srcImg = imread("./castle.jpg", 0);
            Mat dstLaplacian, dstRobert, dstSobel;

            //Laplacian
            dstLaplacian = Smooth_Gray(srcImg, laplacian);
            //dstLaplacian = Add(dstLaplacian, srcImg, 1);   //将提取的边缘与原图叠加

            //Robert
            dstRobert = Smooth_Gray(srcImg, Robert_horizontal);
            dstRobert = Add(Smooth_Gray(srcImg, Robert_vertical), dstRobert, 1);
            //dstRobert = Add(Smooth_Gray(srcImg, Robert_diagonal),dstRobert, 1);
            //dstRobert = Add(Smooth_Gray(srcImg, Robert_anti_diagonal),dstRobert, 1);

            //dstRobert = Add(dstRobert, srcImg, 1);

            //Sobel
            dstSobel = Smooth_Gray(srcImg, Sobel_diagonal);
            dstSobel = Add(Smooth_Gray(srcImg, Sobel_anti_diagonal), dstSobel, 1);

            //dstSobel = Add(dstSobel, srcImg, 1);


            imshow("srcImg", srcImg);
            imshow("laplace", dstLaplacian);
            imshow("robert", dstRobert);
            imshow("dstsobel", dstSobel);
            waitKey();
            return 0;
            break;
        }
        case 4:{
            srcImg = imread("./castle.jpg", 0);
            Mat mask;
            int k;
            cout << "请输入高斯模版尺寸(3/5/9)" << endl;
            cin >> size;
            vector<vector<double> > Gaussian(size, vector<double>(size));
            GenerateGaussian(Gaussian, size);   //生成高斯模版
            mask = Smooth_Gray(srcImg, Gaussian);  //利用高斯模版进行平滑
            mask = Add(srcImg, mask, -1);
            imshow("mask", mask);
            cout << "请输入权重系数: " << endl;
            cin >> k;
            dstImg = Add(srcImg, mask, k);
            break;
        }
        case 5:{
            srcImg = imread("./castle.jpg", 1);
            cout << "请输入均值模版尺寸(3/5/9)" << endl;
            cin >> size;
            vector<vector<double> > Arv;
            Arv = GenerateArv(size);
            vector<Mat> rgbChannels(3);
            split(srcImg, rgbChannels);
            for(int ch = 0; ch<3; ch++){
                rgbChannels[ch] = Smooth_Gray(rgbChannels[ch], Arv);
            }
            merge(rgbChannels, dstImg);
            break;
        }
        case 6:{
            srcImg = imread("./castle.jpg", 1);
            cout << "请输入模版尺寸(3/5/9)" << endl;
            cin >> size;
            vector<vector<double> > Gaussian(size, vector<double>(size));
            GenerateGaussian(Gaussian, size);   //生成高斯模版
            vector<Mat> rgbChannels(3);
            split(srcImg, rgbChannels);
            for(int ch = 0; ch<3; ch++){
                rgbChannels[ch] = Smooth_Gray(rgbChannels[ch], Gaussian);
            }
            merge(rgbChannels, dstImg);
            break;
        }
        case 7:{
            srcImg = imread("./castle.jpg", 1);
            Mat dstLaplacian, dstRobert, dstSobel;
            vector<Mat> rgbChannels(3), rbgLaplacian(3), rgbRobert(3), rgbSobel(3);
            split(srcImg, rgbChannels);

            //Laplacian
            for(int ch = 0; ch<3; ch++){
                rbgLaplacian[ch] = Smooth_Gray(rgbChannels[ch], laplacian);
                rbgLaplacian[ch] = Add(rbgLaplacian[ch], rgbChannels[ch], 1);
            }
            merge(rbgLaplacian, dstLaplacian);

            //Robert
            for(int ch = 0; ch<3; ch++){
                rgbRobert[ch] = Smooth_Gray(rgbChannels[ch], Robert_horizontal);
                rgbRobert[ch] = Add(Smooth_Gray(rgbChannels[ch], Robert_vertical), rgbRobert[ch], 1);
                rgbRobert[ch] = Add(rgbChannels[ch], rgbRobert[ch], 1);
            }
            merge(rgbRobert, dstRobert);

            //Sobel
            for(int ch = 0; ch<3; ch++){
                rgbSobel[ch] = Smooth_Gray(rgbChannels[ch],Sobel_diagonal);
                rgbSobel[ch] = Add(Smooth_Gray(rgbChannels[ch], Sobel_anti_diagonal), rgbSobel[ch], 1);
                rgbSobel[ch] = Add(rgbChannels[ch], rgbSobel[ch], 1);
            }
            merge(rgbSobel, dstSobel);

            imshow("srcImg", srcImg);
            imshow("laplace", dstLaplacian);
            imshow("robert", dstRobert);
            imshow("dstsobel", dstSobel);
            waitKey();
            return 0;
            break;
        }
        default:
            break;
    }
    imshow("srcImg", srcImg);
    imshow("dstImg", dstImg);
    waitKey();
    return 0;
}
