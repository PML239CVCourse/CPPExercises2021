#include "blur.h"

#include <libutils/rasserts.h>
//2
double G(double x, double y, double sig){
    return (1/ (2 * M_PI * sig)) * pow(M_E, -(x*x+y*y)/(2*sig*sig));
}

cv::Mat blur(cv::Mat img, double sigma) {
    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    int width = img.cols;
    int height = img.rows;
    cv::Vec3b color = img.at<cv::Vec3b>(0,0);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];
    std::vector<std::vector<double>> kof(5, std::vector<double>(5));
    for (int dj = -2; dj <= 2; ++dj) {
        for (int di = -2; di <= 2; ++di) {
            kof[dj+2][di+2] = G(dj, di, sigma);
        }
    }

    for (int j = 2; j < height-2; ++j) {
        for (int i = 2; i < width-2; ++i) {
            float b = 0;
            float g = 0;
            float r = 0;
            for (int dj = -2; dj <= 2; ++dj) {
                for (int di = -2; di <= 2; ++di) {
                    b += img.at<cv::Vec3b>(j + dj, i + di)[0] * kof[dj+2][di+2];
                    g += img.at<cv::Vec3b>(j + dj, i + di)[1] * kof[dj+2][di+2];
                    r += img.at<cv::Vec3b>(j + dj, i + di)[2] * kof[dj+2][di+2];
                }
            }
            if(b > 255)
                b = 255;
            if(g > 255)
                g = 255;
            if(r > 255)
                r = 255;
            img.at<cv::Vec3b>(j,i) = {(unsigned char)b,(unsigned char)g,(unsigned char)r};
        }
    }
    return img;
}
