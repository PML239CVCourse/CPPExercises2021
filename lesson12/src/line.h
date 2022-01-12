#pragma once

#include <opencv2/highgui.hpp>

// эта функция рисует на картинке указанные точки
// при этом если картинка пустая - эта функция должна увеличить картинку до размера в который впишутся все точки
void plotPoints(cv::Mat &img, std::vector<cv::Point2f> points, double scale, cv::Scalar color);

std::vector<cv::Point2f> generateRandomPoints(int n,
                                          double fromX, double toX,
                                          double fromY, double toY);

// этот тип описывает прямую - множество таких точек x,y что выполняется уравнение вида a*x+b*y+c=0
class Line {
public:
    double a, b, c;
    Line(double a, double b, double c) : a(a), b(b), c(c)
    {}

    void plot(cv::Mat &img, double scale, cv::Scalar color); // метод прямой позволяющий нарисовать ее на картинке (т.е. на простом графике)

    double getYFromX(double x); // метод прямой позволяющий рассчитать точку на прямой (найти y по x)

    std::vector<cv::Point2f> generatePoints(int n,
                                        double fromX, double toX,
                                        double gaussianNoiseSigma); // метод прямой генерирующий n точек неподалеку от прямой
};

Line fitLineFromTwoPoints(cv::Point2f a, cv::Point2f b);

Line fitLineFromNPoints(std::vector<cv::Point2f> points);

Line fitLineFromNNoisyPoints(std::vector<cv::Point2f> points);

std::ostream& operator << (std::ostream& os, const Line& line);
