#pragma once

#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)

cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat addManyBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground, int n);

cv:: Mat stretch_unicorn_to_castle(cv:: Mat background, cv:: Mat unicorn);

cv:: Mat multicolor_unicorn(cv:: Mat object);

cv:: Mat replace_pixel_with_transparent (cv:: Mat object, cv:: Mat background, std::pair<int, int> p, std::vector<std::pair<int, int>> history);
