#include "blur.h"

#include <libutils/rasserts.h>


int getAverageColorPart(int num_of_color, cv::Mat& img, int rad, int i0, int j0){
    rassert(!img.empty(), "empty image!");
    int width = img.cols;
    int height = img.rows;
    for(int i_rad = -rad; i_rad <= rad; i_rad++){
        for(int j_rad = -rad; j_rad <= rad; j_rad++){
                //if           to check the size of array
            //cv::Vec3b color = img.at<cv::Vec3b>(j_rad + ii, i);
        }
    }
}


cv::Mat blur(cv::Mat img, double sigma) {
    rassert(!img.empty(), "empty image!");
    int width = img.cols;
    int height = img.rows;
    // TODO формулу весов можно найти тут:
    // https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D0%B7%D0%BC%D1%8B%D1%82%D0%B8%D0%B5_%D0%BF%D0%BE_%D0%93%D0%B0%D1%83%D1%81%D1%81%D1%83
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            cv::Vec3b color = img.at<cv::Vec3b>(j, i);
            color[0] = getAverageColorPart(0, img, static_cast<int>(3 * sigma), i, j);
        }
    }
    return img;
}
