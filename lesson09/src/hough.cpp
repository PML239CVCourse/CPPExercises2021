#include "hough.h"

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

double toRadians(double degrees)
{
    const double PI = 3.14159265358979323846264338327950288;
    return degrees * PI / 180.0;
}

double estimateR(double x0, double y0, double theta0radians)
{
    double r0 = x0 * cos(theta0radians) + y0 * sin(theta0radians);
    return r0;
}


cv::Mat buildHough(cv::Mat sobel) {
    rassert(sobel.type() == CV_32FC1, 237128273918006);


    int width = sobel.cols;
    int height = sobel.rows;


    int max_theta = 360;
    int max_r = static_cast<int>(sqrt(width * width + height * height)) - 1;


    cv::Mat accumulator(max_r, max_theta, CV_32FC1, 0.0f);

    float summary_strenth = 0;
    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            float strength = sobel.at<float>(y0, x0);
            summary_strenth += strength;
        }
    }


    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            float strength = sobel.at<float>(y0, x0);


            for (int theta0 = 0; theta0 + 1 < max_theta; ++theta0) {

                double theta0radians = toRadians(theta0);
                int r0 = (int) round(estimateR(x0, y0, theta0radians));
                if (r0 < 0 || r0 >= max_r)
                    continue;



                float color_hough = accumulator.at<float>(r0, theta0);
                color_hough += strength;
                accumulator.at<float>(r0, theta0) = color_hough;
            }
        }
    }



    return accumulator;
}

std::vector<PolarLineExtremum> findLocalExtremums(cv::Mat houghSpace)
{
    rassert(houghSpace.type() == CV_32FC1, 234827498237080);

    const int max_theta = 360;
    rassert(houghSpace.cols == max_theta, 233892742893082);
    const int max_r = houghSpace.rows;

    std::vector<PolarLineExtremum> winners;

    for (int theta = 0; theta < max_theta; ++theta) {
        for (int r = 0; r < max_r; ++r) {
            if(theta == 0 || theta == max_theta)
                continue;
            if(r == 0 || r == max_r)
                continue;

            bool is_ok = true;
            float this_votes = houghSpace.at<float>(r, theta);
            for(int st_r = -1; st_r <= 1; st_r++){
                for(int st_theta = -1; st_theta <= 1; st_theta++){
                    float votes_tmp = houghSpace.at<float>(r + st_r, theta + st_theta);
                    if(votes_tmp > this_votes){
                        is_ok = false;
                        break;
                    }
                }
            }

            if(is_ok){
                PolarLineExtremum line(theta, r, this_votes);
                winners.push_back(line);
            }
        }
    }

    return winners;
}

std::vector<PolarLineExtremum> filterStrongLines(std::vector<PolarLineExtremum> allLines, double thresholdFromWinner)
{
    std::vector<PolarLineExtremum> strongLines;

    PolarLineExtremum polarLineMax(0, 0, 0.0f);

    for(auto line : allLines){
        if(line.votes > polarLineMax.votes){
            polarLineMax = PolarLineExtremum(line.theta, line.r, line.votes);
        }
    }

    for(auto line : allLines){
        if(line.votes >= polarLineMax.votes * thresholdFromWinner){
            strongLines.push_back(line);
        }
    }

    return strongLines;
}

cv::Mat drawCirclesOnExtremumsInHoughSpace(cv::Mat houghSpace, std::vector<PolarLineExtremum> lines, int radius)
{
    // TODO Доделайте эту функцию - пусть она скопирует картинку с пространством Хафа и отметит на ней красным кружком указанного радиуса (radius) места где были обнаружены экстремумы (на базе списка прямых)

    // делаем копию картинки с пространством Хафа (чтобы не портить само пространство Хафа)
    cv::Mat houghSpaceWithCrosses = houghSpace.clone();

    // проверяем что пространство состоит из 32-битных вещественных чисел (т.е. картинка одноканальная)
    rassert(houghSpaceWithCrosses.type() == CV_32FC1, 347823472890137);

    // но мы хотим рисовать КРАСНЫЙ кружочек вокруг найденных экстремумов, а значит нам не подходит черно-белая картинка
    // поэтому ее надо преобразовать в обычную цветную BGR картинку
    cv::cvtColor(houghSpaceWithCrosses, houghSpaceWithCrosses, cv::COLOR_GRAY2BGR);
    // проверяем что теперь все хорошо и картинка трехканальная (но при этом каждый цвет - 32-битное вещественное число)
    rassert(houghSpaceWithCrosses.type() == CV_32FC3, 347823472890148);

    for (int i = 0; i < lines.size(); ++i) {
        PolarLineExtremum line = lines[i];


        cv::Point point(line.r, line.theta);
        cv::Scalar color(0, 0, 255); //red color
        cv::circle(houghSpaceWithCrosses, point, radius, color);
        // Пример как рисовать кружок в какой-то точке (закомментируйте его):
        /*cv::Point point(100, 50);
        cv::Scalar color(0, 0, 255); // BGR, т.е. красный цвет
        cv::circle(houghSpaceWithCrosses, point, 3, color);*/

        // TODO отметьте в пространстве Хафа красным кружком радиуса radius экстремум соответствующий прямой line
    }

    return houghSpaceWithCrosses;
}

cv::Point PolarLineExtremum::intersect(PolarLineExtremum that)
{
    // Одна прямая - наш текущий объект (this) у которого был вызван этот метод, у этой прямой такие параметры:
    double theta0 = this->theta;
    double r0 = this->r;

    // Другая прямая - другой объект (that) который был передан в этот метод как аргумент, у этой прямой такие параметры:
    double theta1 = that.theta;
    double r1 = that.r;

    // TODO реализуйте поиск пересечения этих двух прямых, напоминаю что формула прямой описана тут - https://www.polarnick.com/blogs/239/2021/school239_11_2021_2022/2021/11/02/lesson8-hough-transform.html
    // после этого загуглите как искать пересечение двух прямых, пример запроса: "intersect two 2d lines"
    // и не забудьте что cos/sin принимают радианы (используйте toRadians)

    double num = (r0 / sin(toRadians(theta0))) - (r1 / sin(toRadians(theta1)));
    double denom = (1 / tan(toRadians(theta0))) - (1 / tan(toRadians(theta1)));

    int x = num / denom;
    int y = (r0 / sin(toRadians(theta0))) - (num / denom) / tan(toRadians(theta0));

    return cv::Point(x, y);
}

// TODO Реализуйте эту функцию - пусть она скопирует картинку и отметит на ней прямые в соответствии со списком прямых
cv::Mat drawLinesOnImage(cv::Mat img, std::vector<PolarLineExtremum> lines)
{
    // делаем копию картинки (чтобы при рисовании не менять саму оригинальную картинку)
    cv::Mat imgWithLines = img.clone();

    // проверяем что картинка черно-белая (мы ведь ее такой сделали ради оператора Собеля) и 8-битная
    rassert(imgWithLines.type() == CV_8UC1, 45728934700167);

    // но мы хотим рисовать КРАСНЫЕ прямые, а значит нам не подходит черно-белая картинка
    // поэтому ее надо преобразовать в обычную цветную BGR картинку с 8 битами в каждом пикселе
    cv::cvtColor(imgWithLines, imgWithLines, cv::COLOR_GRAY2BGR);
    rassert(imgWithLines.type() == CV_8UC3, 45728934700172);

    // выпишем размер картинки
    int width = imgWithLines.cols;
    int height = imgWithLines.rows;

    for (int i = 0; i < lines.size(); ++i) {
        PolarLineExtremum line = lines[i];

        // нам надо найти точки на краях картинки
        cv::Point pointA;
        cv::Point pointB;

        // TODO создайте четыре прямых соответствующих краям картинки (на бумажке нарисуйте картинку и подумайте какие theta/r должны быть у прямых?):
        // напоминаю - чтобы посмотреть какие аргументы требует функция (или в данном случае конструктор объекта) - нужно:
        // 1) раскомментировать эти четыре строки ниже
        // 2) поставить каретку (указатель где вы вводите новые символы) внутри скобок функции (или конструктора, т.е. там где были три вопроса: ???)
        // 3) нажать Ctrl+P чтобы показать список параметров (P=Parameters)
//        PolarLineExtremum leftImageBorder(???);
//        PolarLineExtremum bottomImageBorder(???);
//        PolarLineExtremum rightImageBorder(???);
//        PolarLineExtremum topImageBorder(???);

        // TODO воспользуйтесь недавно созданной функций поиска пересечения прямых чтобы найти точки пересечения краев картинки:
//        pointA = line.intersect(leftImageBorder);
//        pointB = line.intersect(rightImageBorder);

        // TODO а в каких случаях нужно использовать пересечение с верхним и нижним краем картинки?
//        pointA = line.intersect(???);
//        pointB = line.intersect(???);

        // TODO переделайте так чтобы цвет для каждой прямой был случайным (чтобы легче было различать близко расположенные прямые)
        cv::Scalar color(0, 0, 255);
        cv::line(imgWithLines, pointA, pointB, color);
    }

    return imgWithLines;
}
