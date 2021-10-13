#pragma once

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)
#include <tuple>
#include <ctime>
#include <math.h>
#include <vector>

cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat addUnicornAtRandomPlace(cv::Mat object, cv::Mat largeBackground);

cv::Mat replaceInRandomColors(cv::Mat object);

//cv::Mat initPicture(cv::Mat object);

bool isFilledBy1OR0(std::vector<std::vector<int>>& arr, int idx_i, int idx_j, int i_max, int j_max, int symb);

cv::Mat videoWithBackground(cv::Mat object, cv::Mat init_pict);

cv::Mat videoWithoutInterference(cv::Mat object, std::vector<std::vector<int>>& difference_in_colors);