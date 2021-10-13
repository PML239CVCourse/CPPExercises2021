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
    cv::Mat dxy = sobelDXY(convertBGRToGray(img)); // обратите внимание что внутри ждут черно-белую картинку, значит нашу картинку надо перед Собелем преобразовать

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

}

struct MyVideoContent {
    cv::Mat frame;
    int lastClickX;
    int lastClickY;
};

void camtest(){
    MyVideoContent content;
    int updateDelay = 10;
    cv::VideoCapture video(0);
    while (video.isOpened() && cv::waitKey(updateDelay) != ' ' && cv::waitKey(updateDelay) != 27) { // пока видео не закрылось - бежим по нему
        bool isSuccess = video.read(content.frame); // считываем из видео очередной кадр
        cv::imshow("video", convertDXYToGradientLength(sobelDXY(convertBGRToGray(content.frame)))); // покаызваем очередной кадр в окошке
    }
}
int main() {
    try {
        testBGRToGray();

        for (int i = 1; i <= 4; ++i) {
            testSobel("line0" + std::to_string(i));
        }

        for (int i = 1; i <= 4; ++i) {
            testSobel("line1" + std::to_string(i));
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
