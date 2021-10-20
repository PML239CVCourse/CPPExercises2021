#pragma once

#include <opencv2/highgui.hpp>

cv::Mat blur(cv::Mat img, double sigma);
float Gauss(int x, int y, double sigma);
