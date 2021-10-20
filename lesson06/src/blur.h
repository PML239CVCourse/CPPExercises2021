#pragma once

#include <opencv2/highgui.hpp>

cv::Mat blur(cv::Mat img, double sigma);

int getAverageColorPart(int num_of_color, cv::Mat& img, int rad, int i0, int j0);