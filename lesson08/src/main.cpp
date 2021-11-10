#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hough.h"

#include <opencv2/imgproc.hpp>


void test(std::string name) {
    std::cout << "Processing image " << name << ".jpg..." << std::endl;

    std::string full_path = "lesson05/data/" + name + ".jpg";
    cv::Mat img = cv::imread(full_path);
    cv::imwrite("lesson08/resultsData/" + name + "_0.png", img); // сохраним еще раз изначальную картинку в папку с результатами чтобы была под рукой
    rassert(!img.empty(), 238982391080010);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого

    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    // замечаем что мы ведь забыли взять абсолютное значение градиента!
    // давайте это исправим:
    grad_x = cv::abs(grad_x);
    grad_y = cv::abs(grad_y);

    cv::Mat sobel_strength(img.rows, img.cols, CV_32FC1, 0.0f);
    // теперь хочется заполнить sobel_strength силой градиента с учетом обеих осей, т.е. что-то вроде sobel_strength=sqrt(grad_x^2+grad_x^2):
    for (int j = 0; j < sobel_strength.rows; ++j) {
        for (int i = 0; i < sobel_strength.cols; ++i) {
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            float gradient_strength = sqrtf(dx*dx+dy*dy);
            sobel_strength.at<float>(j, i) = gradient_strength;
        }
    }
    cv::imwrite("lesson08/resultsData/" + name + "_1_sobel_strength.png", sobel_strength);

    // TODO
    // Эта функция по картинке с силами градиентов (после свертки оператором Собеля) строит пространство Хафа
    // Вы можете либо взять свою реализацию из прошлого задания, либо взять мою заготовку которая предложена внутри этой функции
    // и довезти ее до ума
    cv::Mat hough = buildHough(sobel_strength);

    // но яркость иногда сильно больше чем 255
    // поэтому найдем максимальную яркость (max_accumulated) среди всей матрицы hough и после этого отнормируем всю картинку:
    float max_accumulated = 0.0f;
    for (int j = 0; j < hough.rows; ++j) {
        for (int i = 0; i < hough.cols; ++i) {
            max_accumulated = std::max(max_accumulated, hough.at<float>(j, i));
        }
    }

    // заменим каждый пиксель с яркости X на яркость X*255.0f/max_accumulated (т.е. уменьшим диапазон значений)
    cv::imwrite("lesson08/resultsData/" + name + "_2_hough_normalized.png", hough*255.0f/max_accumulated);

// TODO здесь может быть полезно сгладить пространство Хафа, см. комментарии на сайте - https://www.polarnick.com/blogs/239/2021/school239_11_2021_2022/2021/11/09/lesson9-hough2-interpolation-extremum-detection.html

// TODO реализуйте функцию которая ищет и перечисляет локальные экстремумы - findLocalExtremums(...)
    std::vector<PolarLineExtremum> lines = findLocalExtremums(hough);

// TODO реализуйте фильтрацию прямых - нужно оставлять только те прямые, у кого много голосов (реализуйте функцию filterStrongLines(...) ):
//    double thresholdFromWinner = 0.5; // хотим оставить только те прямые у кого не менее половины голосов по сравнению с самой популярной прямой
//    lines = filterStrongLines(lines, thresholdFromWinner);

    std::cout << "Found " << lines.size() << " extremums:" << std::endl;
    for (int i = 0; i < lines.size(); ++i) {
        std::cout << "  Line #" << (i + 1) << " theta=" << lines[i].theta << " r=" << lines[i].r << " votes=" << lines[i].votes << std::endl;
    }
}


int main() {
    try {
        test("line01");

//        test("line02");

//        test("line11");

//        test("line12");

//        test("line21_water_horizont");

//        test("multiline1_paper_on_table");

//        test("multiline2_paper_on_table");

//        test("valve");

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

