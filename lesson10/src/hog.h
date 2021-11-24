#pragma once

#include <opencv2/highgui.hpp>

#include <vector>

// Здесь мы будем хранить параметры нашего алгоритма:
#define NBINS 8 // число корзин (т.е. количество разных направлений угла которые мы рассматриваем)


typedef std::vector<double> HoG;

// TODO эту функцию вам надо реализовать
HoG buildHoG(cv::Mat grad_x, cv::Mat grad_y); // принимает на вход градиенты по обеим осям

HoG buildHoG(cv::Mat img); // принимает на вход оригинальную картинку и вызывает функцию buildHoG объявленную выше

// TODO эту функцию вам надо реализовать
std::ostream& operator << (std::ostream& os, const HoG& hog);

// TODO эту функцию вам надо реализовать
double distance(HoG a, HoG b);
