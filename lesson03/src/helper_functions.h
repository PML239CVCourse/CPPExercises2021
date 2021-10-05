#pragma once

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)

cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackgroundNtimes(cv::Mat object, cv::Mat largeBackground, int n);

cv::Mat makeAllBlackPixelsRandom(cv::Mat object);

cv::Mat makePixelsRed(std::vector<int> vectorX, std::vector<int> vectorY, cv::Mat frame);

cv::Mat makePixelsClear(int X, int Y, cv::Mat frame, cv::Mat background);

cv::Mat scaleImage(cv::Mat object, cv::Mat largeBackground);

