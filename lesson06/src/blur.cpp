#include "blur.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <libutils/rasserts.h>
#include <vector>
using namespace std;


cv::Mat blur(cv::Mat img, double sigma) {

    int width = img.cols;
    int height = img.rows;
    cv::Mat BlurImg(height, width, CV_32FC3);

    double sum = 0;
    for(int x = -sigma; x <= sigma; ++x) {
        for (int y = -sigma; y <= sigma; ++y) {
            double k = (1.0 / (2*M_PI*sigma*sigma)) * exp((-(x*x)-(y*y)) / (2*sigma*sigma));
            sum += k;
        }
    }

    int d = 2*sigma + 1;
    vector<vector<double>> g(d, vector <double> (d));
    for(int y = -sigma ; y <= sigma; ++y){
        for(int x = -sigma ; x <= sigma; ++x){
            g[x+sigma][y+sigma] = ((1.0 / (2*M_PI*sigma*sigma)) * exp((-(x*x)-(y*y)) / (2*sigma*sigma))) / sum ;
        }
    }

    for(int j = sigma; j < height - sigma; ++j) {
        for (int i = sigma; i < width - sigma; ++i) {

            double blue = 0.0, green = 0.0, red = 0.0;

            for(int y = 0; y < d; ++y){
                for(int x = 0; x < d; ++x){

                    cv::Vec3b c = img.at<cv::Vec3b>(j + y - sigma, x + i - sigma);

                    blue += c[0]*g[y][x];
                    green += c[1]*g[y][x];
                    red += c[2]*g[y][x];
                }
            }
            BlurImg.at<cv::Vec3f>(j, i) = cv::Vec3f(blue, green, red);
        }
    }

    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    return BlurImg;
}
