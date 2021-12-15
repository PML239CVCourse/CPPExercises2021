#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hough.h"

#include <opencv2/imgproc.hpp>


void test(std::string name) {
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

