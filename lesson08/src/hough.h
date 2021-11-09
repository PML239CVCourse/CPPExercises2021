#pragma once

#include <opencv2/highgui.hpp>

// Этот класс представляет прямую в полярных координатах + число голосов которые были за эту прямую отданы (мерило ее силы и популярности)
class PolarLineExtremum {
public:
    double theta;
    double r;

    double votes;

    PolarLineExtremum(double theta, double r, double votes)
    {
        this->theta = theta;
        this->r = r;
        this->votes = votes;
    }
};

// Эта функция по картинке с силами градиентов (после свертки оператором Собеля) строит пространство Хафа
// Вы можете либо взять свою реализацию из прошлого задания, либо взять мою заготовку которая предложена внутри этой функции
cv::Mat buildHough(cv::Mat sobel);

// Эта функция проходит по всему пространству Хафа и извлекает перечень локальных экстремумов - найденных прямых
std::vector<PolarLineExtremum> findLocalExtremums(cv::Mat houghSpace);

// Эта функция по множеству всех найденных локальных экстремумов (прямых) находит самую популярную прямую
// и возвращает только вектор из тех прямых, что не сильно ее хуже (набрали хотя бы thresholdFromWinner голосов от победителя, т.е. например половину)
std::vector<PolarLineExtremum> filterStrongLines(std::vector<PolarLineExtremum> allLines, double thresholdFromWinner);
