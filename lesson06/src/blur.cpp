#include "blur.h"

#include <libutils/rasserts.h>

cv::Mat blur(cv::Mat img, double sigma) {
    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    int width = img.cols;
    int height = img.rows;
    cv::Mat gaus(2*sigma+1, 2*sigma+1, CV_32FC1);
    cv::Mat myNewImage(height, width, CV_8UC3, cv::Scalar(0, 0, 0));
    float sum = 0;
    for(int x=-sigma;x<=sigma;x++)
        for(int y=-sigma;y<=sigma;y++){
            gaus.at<float>(sigma-x,sigma-y)=1./2./3.141592/(float)sigma/(float)sigma*exp(-(x*x+y*y)/2./(float)sigma/(float)sigma);
            sum+=gaus.at<float>(sigma-x,sigma-y);
            }
    float koef = 1/sum;
    for(int i = sigma;i<height-sigma;i++)
        for(int j = sigma;j<width-sigma;j++)
            for(int x=-sigma;x<=sigma;x++)
                for(int y=-sigma;y<=sigma;y++)
                {
                    float g = gaus.at<float>(sigma+x,sigma+y)*koef;
                    cv::Vec3b color = img.at<cv::Vec3b>(i+x, j+y);
                    float blue = (float)color[0];
                    float green = (float)color[1];
                    float red = (float)color[2];
                    cv::Vec3b color2 = myNewImage.at<cv::Vec3b>(i, j);
                    unsigned char blue2 = color2[0]+(unsigned char)(blue*g);
                    unsigned char green2 = color2[1]+(unsigned char)(green*g);
                    unsigned char red2 = color2[2]+(unsigned char)(red*g);
                    cv::Vec3b new_color = {blue2, green2, red2};
                    myNewImage.at<cv::Vec3b>(i, j) = new_color;
                }
    return myNewImage;
}
