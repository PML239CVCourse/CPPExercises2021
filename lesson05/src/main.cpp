#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "sobel.h"

#include <opencv2/highgui.hpp>

void testBGRToGray() {
    std::string name = "valve";
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    // TODO реализуйте эту функцию, преобразующую картинку в черно-белую
    cv::Mat gray = convertBGRToGray(img);

    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_grey.jpg", gray);
}

void testSobel(const std::string &name) {
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    rassert(!img.empty(), 23981920813);

    // TODO реализуйте функцию считающую применение оператора Собеля к картинке
    // т.е. посчитайте производную по x и по y (в каждом пикселе хранятся две эти производные)
    cv::Mat dxy = sobelDXY(img); // обратите внимание что внутри ждут черно-белую картинку, значит нашу картинку надо перед Собелем преобразовать

    cv::Mat dx = convertDXYToDX(dxy); // TODO реализуйте функцию которая вытаскивает силу производной по x (ее абсолютное значение)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_dx.jpg", dx);

    cv::Mat dy = convertDXYToDY(dxy); // TODO реализуйте функцию которая вытаскивает силу производной по y (ее абсолютное значение)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_dy.jpg", dy);

    cv::Mat gradientStrength = convertDXYToGradientLength(dxy); // TODO реализуйте функцию которая считает силу градиента в каждом пикселе
    // точнее - его длину, ведь градиент - это вектор (двухмерный, ведь у него две компоненты), а у вектора всегда есть длина - sqrt(x^2+y^2)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    cv::imwrite("lesson05/resultsData/" + name + "_gradientLength.jpg", gradientStrength);
    // для valve.jpg должно быть похоже на картинку с википедии - https://ru.wikipedia.org/wiki/%D0%9E%D0%BF%D0%B5%D1%80%D0%B0%D1%82%D0%BE%D1%80_%D0%A1%D0%BE%D0%B1%D0%B5%D0%BB%D1%8F
}

int main() {
    try {
        testBGRToGray();

        for (int i = 1; i <= 4; ++i) {
            testSobel("line0" + std::to_string(i));
        }

        for (int i = 1; i <= 4; ++i) {
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
