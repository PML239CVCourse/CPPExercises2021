#pragma once

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)

cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackgroundNTimes(cv::Mat object, cv::Mat largeBackground);

cv::Mat fillBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat doTask4(cv::Mat object, cv::Mat largeBackground, cv::Vec3b color, double dis);
