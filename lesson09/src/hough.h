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

    // TODO Реализуйте эту функцию - она должна находить точку пересечения двух прямых (прямой this - т.е. объекта у которого вызван метод, и прямой that - т.е. объекта с которым делается пересечение)
    cv::Point intersect(PolarLineExtremum that);
};

// Эта функция по картинке с силами градиентов (после свертки оператором Собеля) строит пространство Хафа
// Вы можете либо взять свою реализацию из прошлого задания, либо взять мою заготовку которая предложена внутри этой функции
cv::Mat buildHough(cv::Mat sobel);

// Эта функция проходит по всему пространству Хафа и извлекает перечень локальных экстремумов - найденных прямых
std::vector<PolarLineExtremum> findLocalExtremums(cv::Mat houghSpace);

// Эта функция по множеству всех найденных локальных экстремумов (прямых) находит самую популярную прямую
// и возвращает только вектор из тех прямых, что не сильно ее хуже (набрали хотя бы thresholdFromWinner голосов от победителя, т.е. например половину)
std::vector<PolarLineExtremum> filterStrongLines(std::vector<PolarLineExtremum> allLines, double thresholdFromWinner);

// TODO Реализуйте эту функцию - пусть она скопирует картинку с пространством Хафа и отметит на ней красным кружком указанного радиуса (radius) места где были обнаружены экстремумы (на базе списка прямых)
cv::Mat drawCirclesOnExtremumsInHoughSpace(cv::Mat houghSpace, std::vector<PolarLineExtremum> lines, int radius);

// TODO Реализуйте эту функцию - пусть она скопирует картинку и отметит на ней прямые в соответствии со списком прямых
cv::Mat drawLinesOnImage(cv::Mat img, std::vector<PolarLineExtremum> lines);
