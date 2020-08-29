#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "denoising.h"
using namespace cv;
using namespace std;

#define COLOR 0             //处理彩色图像
#define ARV 0
#define GEO 0
#define HAR 0
#define CONTRA_HAR 0
#define ARVCOLOR 0
#define GEOCOLOR 0

int main() {
    Mat srcImg, dstImg, dstGaussian, dstSalt, dstPepper, dstSalt_Pepper;

#if COLOR == 0
    srcImg = imread("castle.jpg", 0);
    dstGaussian = addGaussianNoise(srcImg);
    dstSalt = addSalt(srcImg,0.01);
    dstPepper = addPepper(srcImg, 0.01);
    dstSalt_Pepper = addSalt(srcImg, 0.01);
    dstSalt_Pepper = addPepper(dstSalt_Pepper, 0.01);
    imwrite("../pic/Gaussian.jpg", dstGaussian);
    imwrite("../pic/Pepper.jpg", dstPepper);
    imwrite("../pic/Salt.jpg", dstSalt);
    imwrite("../pic/Salt_Pepper.jpg", dstSalt_Pepper);
#endif

    int size;
    printf("请输入实验名：1 均值滤波；"
           "2 中值滤波；"
           "3 自适应均值滤波；"
           "4 自适应中值滤波；"
           "5 彩色图像均值滤波; \n");
    int exam;
    cin >> exam;
    switch(exam){
        case 1:{

#if ARV > 0
            vector<vector<double> > Arv;
            Arv = GenerateArv(5);
            dstGaussian = Smooth(dstGaussian,Arv);
            dstPepper = Smooth(dstPepper,Arv);
            dstSalt = Smooth(dstSalt,Arv);
            dstSalt_Pepper = Smooth(dstSalt_Pepper,Arv);
#endif

#if GEO > 0
            dstGaussian = Geometric(dstGaussian, 5);
            dstPepper = Geometric(dstPepper, 5);
            dstSalt = Geometric(dstSalt, 5);
            dstSalt_Pepper = Geometric(dstSalt_Pepper, 5);
#endif

#if HAR > 0
            dstGaussian = Harmonic(dstGaussian, 5);
            dstPepper = Harmonic(dstPepper, 5);
            dstSalt = Harmonic(dstSalt, 5);
            dstSalt_Pepper = Harmonic(dstSalt_Pepper, 5);
#endif

#if CONTRA_HAR > 0
            cout << "请输入滤波器阶数p" << endl;
            int p;
            cin >> p;
            dstGaussian = ContraHarmonic(dstGaussian, 5, p);
            dstPepper = ContraHarmonic(dstPepper, 5, p);
            dstSalt = ContraHarmonic(dstSalt, 5, p);
            dstSalt_Pepper = ContraHarmonic(dstSalt_Pepper, 5, p);
#endif
            break;
        }
        case 2:{
            cout << "请输入模版尺寸" << endl;
            int size;
            cin >> size;
            dstGaussian = Median(dstGaussian, size);
            dstPepper = Median(dstPepper, size);
            dstSalt = Median(dstSalt, size);
            dstSalt_Pepper = Median(dstSalt_Pepper, size);
            break;
        }
        case 3:{
            dstGaussian = Arv_Adaptive(dstGaussian, 7, 1);
            dstPepper = Arv_Adaptive(dstPepper, 7, 1);
            dstSalt = Arv_Adaptive(dstSalt, 7, 1);
            dstSalt_Pepper = Arv_Adaptive(dstSalt_Pepper, 7, 1);
        }
        case 4:{
            dstGaussian = Mid_Adaptive(dstGaussian, 7);
            dstPepper = Mid_Adaptive(dstPepper, 7);
            dstSalt = Mid_Adaptive(dstSalt, 7);
            dstSalt_Pepper = Mid_Adaptive(dstSalt_Pepper, 7);
            break;
        }
        case 5:{
            srcImg = imread("castle.jpg", 1);
            vector<Mat> rgbsrcImg(3), rgbGaussian(3), rgbPepper(3), rgbSalt(3), rgbSalt_Pepper(3);
            split(srcImg, rgbsrcImg);

            for(int ch = 0; ch<3; ch++){
                rgbGaussian[ch] = addGaussianNoise(rgbsrcImg[ch]);
                rgbPepper[ch] = addPepper(rgbsrcImg[ch], 0.15);
                rgbSalt[ch] = addSalt(rgbsrcImg[ch], 0.15);
                rgbSalt_Pepper[ch] = addSalt(rgbsrcImg[ch], 0.15);
                rgbSalt_Pepper[ch] = addPepper(rgbSalt_Pepper[ch], 0.15);
            }

#if ARVCOLOR > 0
            vector<vector<double> > Arv;
            Arv = GenerateArv(5);
            for(int ch = 0; ch<3; ch++){
                rgbGaussian[ch] = Smooth(rgbGaussian[ch], Arv);
                rgbPepper[ch] = Smooth(rgbPepper[ch], Arv);
                rgbSalt[ch] = Smooth(rgbSalt[ch], Arv);
                rgbSalt_Pepper[ch] = Smooth(rgbSalt_Pepper[ch], Arv);
            }
#endif

#if GEOCOLOR > 0
            for(int ch = 0; ch<3; ch++){
                rgbGaussian[ch] = Geometric(rgbGaussian[ch], 5);
                rgbPepper[ch] = Geometric(rgbPepper[ch], 5);
                rgbSalt[ch] = Geometric(rgbSalt[ch], 5);
                rgbSalt_Pepper[ch] = Geometric(rgbSalt_Pepper[ch], 5);
            }
#endif

            merge(rgbGaussian, dstGaussian);
            merge(rgbPepper, dstPepper);
            merge(rgbSalt, dstSalt);
            merge(rgbSalt_Pepper, dstSalt_Pepper);
            break;
        }
        default:
            break;
    }
    imshow("srcImg", srcImg);
    imshow("GuassianNoise", dstGaussian);
    imshow("PeperNoise", dstPepper);
    imshow("SaltNoise", dstSalt);
    imshow("Salt & Peper Noise", dstSalt_Pepper);
    waitKey();
    return 0;
}
