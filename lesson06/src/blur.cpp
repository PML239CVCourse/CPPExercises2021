#include "blur.h"

#include <libutils/rasserts.h>
#include <cmath>
#include <iostream>

float Gauss(int x, int y, double sigma){
    const double PI = 3.141592653589793;
    return exp(-((float)x*(float)x+(float)y*(float)y)/(2*sigma*sigma))/(2*PI*sigma*sigma);
}

cv::Mat blur(cv::Mat img, double sigma) {
    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    float GaussKoef[5][5] = {
            {Gauss(-2,-2,sigma),Gauss(-1,-2,sigma), Gauss(0,-2,sigma), Gauss(1,-2,sigma), Gauss(2,-2,sigma)},
            {Gauss(-2,-1,sigma),Gauss(-1,-1,sigma), Gauss(0,-1,sigma), Gauss(1,-1,sigma), Gauss(2,-1,sigma)},
            {Gauss(-2,0,sigma),Gauss(-1,0,sigma), Gauss(0,0,sigma), Gauss(1,0,sigma), Gauss(2,0,sigma)},
            {Gauss(-2,1,sigma),Gauss(-1,1,sigma), Gauss(0,1,sigma), Gauss(1,1,sigma), Gauss(2,1,sigma)},
            {Gauss(-2,2,sigma),Gauss(-1,2,sigma), Gauss(0,2,sigma), Gauss(1,2,sigma), Gauss(2,2,sigma)},
            };
    float sum = 0.0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            sum += GaussKoef[i][j];
            std::cout << GaussKoef[i][j] << " ";
        }
        std::cout << std::endl;
    }
    int width = img.cols;
    int height = img.rows;
    cv::Vec3b color = img.at<cv::Vec3b>(0, 0);
    float s0 = 0.0;
    float s1 = 0.0;
    float s2 = 0.0;
    for (int i = 2; i < width-2; ++i) {
        for (int j = 2; j < height-2; ++j) {
            s0 = 0.0;
            s1 = 0.0;
            s2 = 0.0;
//            std::cout << j << " " << height <<  std::endl;
            for (int di = -2; di < 2; ++di) {
                for (int dj = -2; dj < 2; ++dj) {
                    rassert(j+dj >= 0, 12487623876124)
                    rassert(j+dj < height, 12487623876124)
                    rassert(i+di >= 0, 12487623876124)
                    rassert(i+di < width, 12487623876124)
                    std::cout << GaussKoef[i+di][j+dj]*(float)(img.at<cv::Vec3b>(j+dj, i+di)[0]) << std::endl;
                    s0 += GaussKoef[i+di][j+dj]*(float)(img.at<cv::Vec3b>(j+dj, i+di)[0]);
                    s1 += GaussKoef[i+di][j+dj]*(float)(img.at<cv::Vec3b>(j+dj, i+di)[1]);
                    s2 += GaussKoef[i+di][j+dj]*(float)(img.at<cv::Vec3b>(j+dj, i+di)[2]);
                }
            }
            img.at<cv::Vec3b>(j,i)[0] = (unsigned int)(s0/sum);
            img.at<cv::Vec3b>(j,i)[1] = (unsigned int)(s1/sum);
            img.at<cv::Vec3b>(j,i)[2] = (unsigned int)(s2/sum);
        }
    }
    return img;
}
