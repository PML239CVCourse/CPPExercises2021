#pragma once

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)

cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);
cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackgroundN(cv::Mat object, cv::Mat largeBackground,int N);
cv::Mat f(cv::Mat object, cv::Mat largeBackground);
cv::Mat f1(cv::Mat object);
std::vector<std::vector<int>> Dilate(std::vector<std::vector<int>> mas, int r);

std::vector<std::vector<int>> Erode(std::vector<std::vector<int>> mas, int r);
cv::Mat rast(cv::Mat object, cv::Mat largeBackground);


cv::Mat rast1(cv::Mat object, cv::Mat largeBackground, std::vector<int> pix);

cv::Mat baba(cv::Mat largeBackground, cv::Mat object);

cv::Mat rast2(cv::Mat object, cv::Mat largeBackground, cv::Mat fon);