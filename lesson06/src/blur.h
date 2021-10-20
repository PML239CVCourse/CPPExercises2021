#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <opencv2/highgui.hpp>

cv::Mat blur(cv::Mat img, double sigma);
//2
double G(double x, double y, double sig);