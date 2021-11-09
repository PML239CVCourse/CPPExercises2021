#include "blur.h"
#include <math.h>

#define _USE_MATH_DEFINES

#include <libutils/rasserts.h>
#include <iostream>


cv::Mat blur(cv::Mat img, double sigma) {
    const int radius = 3 * sigma + 1;
    cv::Mat weights (2*radius+1, 2*radius+1, CV_64F);
    for (int j = -radius; j <= radius; j++) {
        for (int i = -radius; i <= radius; i++) {
            weights.at<double>(j+radius,i+radius) = exp(-(i * i + j * j) / 2.0 / sigma / sigma) / 2 / 3.14 / sigma / sigma;
        }
    }
    // TODO формулу весов можно найти тут:
    for (int j = 0; j < img.cols; j++) {
        for (int i = 0; i < img.rows; i++) {
            double sumB = 0;
            double sumG = 0;
            double sumR = 0;
            double sumWeight = 0;
            for (int j1 = -radius; j1 <= radius; j1++) {
                for (int i1 = -radius; i1 <= radius; i1++) {
                    if ((j1+j<0)||(i1+i<0)||(j1+j>=img.cols)||(i1+i>=img.rows)) continue;
                    sumWeight = sumWeight + weights.at<double>(j1+radius,i1+radius);
                    cv::Vec3b color = img.at<cv::Vec3b>(i+i1, j+j1);
                    sumB = sumB + color[0] * weights.at<double>(j1+radius,i1+radius);
                    sumG = sumG + color[1] * weights.at<double>(j1+radius,i1+radius);
                    sumR = sumR + color[2] * weights.at<double>(j1+radius,i1+radius);
                }
            }
            sumB = sumB/sumWeight;
            sumG = sumG/sumWeight;
            sumR = sumR/sumWeight;
            img.at<cv::Vec3b>(i, j) = cv::Vec3b(sumB, sumG, sumR);
        }
    }
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    return img;
}
