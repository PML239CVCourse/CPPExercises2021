#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>
#include <string>
#include "blur.h" // TODO реализуйте функцию блюра с произвольной силой размытия в файле blur.cpp

void testSomeBlur() {
    // TODO выберите любую картинку и любую силу сглаживания - проверьте что результат - чуть размытая картинка
    // Входные картинки для тестирования возьмите из предыдущего урока (т.е. по пути lesson05/data/*).
    // Результирующие картинки сохарняйте в эту папку (т.е. по пути lesson06/resultsData/*).

    cv::Mat img = cv::imread("lesson05/data/valve.jpg");
    rassert(!img.empty(), 23981920813);

    cv::Mat img_blur = blur(img.clone(), 3);
    rassert(!img_blur.empty(), 239819208863);
    rassert(img_blur.type() == CV_32FC3, 278179253109);

    cv::imwrite("lesson06/resultsData/valve_blur.jpg", img_blur);
}

void testManySigmas() {
    // TODO возьмите ту же самую картинку но теперь в цикле проведите сглаживание для нескольких разных сигм
    // при этом результирующую картинку сохраняйте с указанием какая сигма использовалась:
    // для того чтобы в название файла добавить значение этой переменной -
    // воспользуйтесь функцией преобразующей числа в строчки - std::to_string(sigma)
    int sigma;
    cv::Mat img = cv::imread("lesson05/data/valve.jpg");

    std::cout << "Vvedite max radius razmytiya: ";
    std::cin >> sigma;

    for(int i=1; i <= sigma; ++i){
        cv::Mat img_blur = blur(img.clone(), i);

        std::string s = std::to_string(i);
        cv::imwrite("lesson06/resultsData/valve_blur" + s + ".jpg", img_blur);
    }

}

int main() {
    try {
        testSomeBlur();
        testManySigmas();

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

