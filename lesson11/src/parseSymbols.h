#pragma once

#include <opencv2/highgui.hpp>
#include <vector>
using namespace std;
std::vector<cv::Mat> splitSymbols(cv::Mat img);
void prep();
string ch(cv::Mat img);