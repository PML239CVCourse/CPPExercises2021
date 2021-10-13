#pragma once

#include <opencv2/highgui.hpp>

cv::Mat convertBGRToGray(cv::Mat img);

cv::Mat sobelDXY(cv::Mat img);

cv::Mat convertDXYToDX(cv::Mat img);
cv::Mat convertDXYToDY(cv::Mat img);
cv::Mat convertDXYToGradientLength(cv::Mat img);
