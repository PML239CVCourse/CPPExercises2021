#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hough.h"

#include <opencv2/imgproc.hpp>


void test(std::string name) {

    std::cout << "Processing image " << name << ".jpg..." << std::endl;

    std::string full_path = "lesson05/data/" + name + ".jpg";
    cv::Mat img = cv::imread(full_path);
    cv::imwrite("lesson08/resultsData/" + name + "_0.png", img);
    rassert(!img.empty(), 238982391080010);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y;
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    grad_x = cv::abs(grad_x);
    grad_y = cv::abs(grad_y);

    cv::Mat sobel_strength(img.rows, img.cols, CV_32FC1, 0.0f);
    for (int j = 0; j < sobel_strength.rows; ++j) {
        for (int i = 0; i < sobel_strength.cols; ++i) {
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            float gradient_strength = sqrtf(dx*dx+dy*dy);
            sobel_strength.at<float>(j, i) = gradient_strength;
        }
    }
    cv::imwrite("lesson08/resultsData/" + name + "_1_sobel_strength.png", sobel_strength);

    cv::Mat hough = buildHough(sobel_strength);
    float max_accumulated = 0.0f;
    for (int j = 0; j < hough.rows; ++j) {
        for (int i = 0; i < hough.cols; ++i) {
            max_accumulated = std::max(max_accumulated, hough.at<float>(j, i));
        }
    }
    cv::imwrite("lesson08/resultsData/" + name + "_2_hough_normalized.png", hough*255.0f/max_accumulated);


    std::vector<PolarLineExtremum> lines = findLocalExtremums(hough);
    double thresholdFromWinner = 0.5; // хотим оставить только те прямые у кого не менее половины голосов по сравнению с самой популярной прямой
    lines = filterStrongLines(lines, thresholdFromWinner);

    std::cout << "Found " << lines.size() << " extremums:" << std::endl;
    for (int i = 0; i < lines.size(); ++i) {
        std::cout << "  Line #" << (i + 1) << " theta=" << lines[i].theta << " r=" << lines[i].r << " votes=" << lines[i].votes << std::endl;
    }


    // TODO 1 скопируйте сюда функцию test из предыдущего задания (lesson08)
    // TODO 2 не забудьте заменить lesson08 в путях отладочных картинок на lesson09:
    // Ctrl+R (это как Ctrl+F но для автоматической замены) -> lesson08 -> lesson09 -> Replace All

    // TODO 3 раскоментируйте эти куски кода и реализуйте эти две функции (обозначение экстремумов в пространстве Хафа и затем рисование самих прямых)
//    int radius = 5;
//    cv::Mat houghWithCircles = drawCirclesOnExtremumsInHoughSpace(hough*255.0f/max_accumulated, lines, radius);
//    cv::imwrite("lesson09/resultsData/" + name + "_4_hough_circles.png", houghWithCircles);
//
//    cv::Mat imgWithLines = drawLinesOnImage(img, lines);
//    cv::imwrite("lesson09/resultsData/" + name + "_5_lines.png", imgWithLines);
}


int main() {
    try {
        // TODO 4 обязательно получите результат на всех картинках:
        test("line01");

        test("line02");

        test("line11");

        test("line12");

        test("line21_water_horizont");

        test("multiline1_paper_on_table");

        test("multiline2_paper_on_table");

        test("valve");

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

