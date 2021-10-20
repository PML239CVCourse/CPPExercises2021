#pragma once

#include <opencv2/highgui.hpp>

cv::Mat convertBGRToGray(cv::Mat img);

cv::Mat sobelDXY(cv::Mat img);

cv::Mat convertDXYToDX(cv::Mat img);
cv::Mat convertDXYToDY(cv::Mat img);
cv::Mat convertDXYToGradientLength(cv::Mat img);


cv::Mat makeAllBlackPixelsBlue(cv::Mat image);
cv::Mat invertImageColors(cv::Mat image);
cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);
cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);
cv::Mat sosibibu(cv::Mat object, cv::Mat largeBackground);
cv::Mat rast(cv::Mat object, cv::Mat largeBackground);
cv::Mat epilepsia(cv::Mat object);
cv::Mat rast1(cv::Mat object, cv::Mat largeBackground, std::vector<int> pix);
cv::Mat baba(cv::Mat largeBackground, cv::Mat object);
cv::Mat rast2(cv::Mat object, cv::Mat largeBackground, cv::Mat fon);
std::vector<std::vector<int>> Dilate(std::vector<std::vector<int>> mas, int r);
std::vector<std::vector<int>> Erode(std::vector<std::vector<int>> mas, int r);