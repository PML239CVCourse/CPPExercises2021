#pragma once
#include <iostream>
#include <opencv2/highgui.hpp> // подключили часть библиотеки OpenCV, теперь мы можем работать с картинками (знаем про тип cv::Mat)
using namespace std;
#include <vector>
cv::Mat makeAllBlackPixelsBlue(cv::Mat image);

cv::Mat invertImageColors(cv::Mat image);

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background);

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground);

cv::Mat getanime(cv::Mat object, cv::Mat largeBackground,int N);

cv::Mat blackrandom(cv::Mat object);

cv::Mat tuda(cv::Mat image,int c,int d);

vector <vector <int> > go(vector <vector <int> > v,int r,double c,int coef);

vector <vector <int> > go2(vector <vector <int> > v,int r,double c,int coef);

vector <vector <int> > go3(vector <vector <int> > v,int r,double c,int coef);