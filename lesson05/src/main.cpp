#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "sobel.h"

#include <opencv2/highgui.hpp>

void testBGRToGray() {
    std::string name = "valve";
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    cv::Mat gray = convertBGRToGray(img);

    cv::imwrite("lesson05/resultsData/" + name + "_grey.jpg", gray);
}

void testSobel(const std::string &name) {
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    // т.е. посчитайте производную по x и по y (в каждом пикселе хранятся две эти производные)
    cv::Mat imggray = convertBGRToGray(img);
    cv::Mat dxy = sobelDXY(imggray); // обратите внимание что внутри ждут черно-белую картинку, значит нашу картинку надо перед Собелем преобразовать

    cv::Mat dx = convertDXYToDX(dxy);
    cv::imwrite("lesson05/resultsData/" + name + "_dx.jpg", dx);

    cv::Mat dy = convertDXYToDY(dxy);
    cv::imwrite("lesson05/resultsData/" + name + "_dy.jpg", dy);

    cv::Mat gradientStrength = convertDXYToGradientLength(dxy);
    // точнее - его длину, ведь градиент - это вектор (двухмерный, ведь у него две компоненты), а у вектора всегда есть длина - sqrt(x^2+y^2)
    cv::imwrite("lesson05/resultsData/" + name + "_gradientLength.jpg", gradientStrength);
    // для valve.jpg должно быть похоже на картинку с википедии - https://ru.wikipedia.org/wiki/%D0%9E%D0%BF%D0%B5%D1%80%D0%B0%D1%82%D0%BE%D1%80_%D0%A1%D0%BE%D0%B1%D0%B5%D0%BB%D1%8F
}

int main() {
    try {
        testBGRToGray();

//        for (int i = 1; i <= 4; ++i) {
//            testSobel("line0" + std::to_string(i));
//        }

        for (int i = 1; i <= 4; ++i) {
            testSobel("line1" + std::to_string(i));
            // TODO сделайте вызов тестирования картинок line11.jpg - line14.jpg
        }

        testSobel("line21_water_horizont");
        testSobel("multiline1_paper_on_table");
        testSobel("multiline2_paper_on_table");

        testSobel("valve");
        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
