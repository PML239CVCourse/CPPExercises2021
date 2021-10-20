#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>
#include <string>
#include "blur.h" // TODO реализуйте функцию блюра с произвольной силой размытия в файле blur.cpp

void testSomeBlur() {
    std::string name = "emperor-vs-horus";
    cv::Mat img = cv::imread("lesson06/data/" + name + ".jpg");
    // TODO выберите любую картинку и любую силу сглаживания - проверьте что результат - чуть размытая картинка
    // Входные картинки для тестирования возьмите из предыдущего урока (т.е. по пути lesson05/data/*).
    // Результирующие картинки сохарняйте в эту папку (т.е. по пути lesson06/resultsData/*).
    cv::Mat bluremp = blur(img,0.1);
    cv::imwrite("lesson06/resultsData/" + name + "_blur_.jpg" , bluremp);
}

void testManySigmas() {
    // TODO возьмите ту же самую картинку но теперь в цикле проведите сглаживание для нескольких разных сигм
    // при этом результирующую картинку сохраняйте с указанием какая сигма использовалась:
    // для того чтобы в название файла добавить значение этой переменной -
    // воспользуйтесь функцией преобразующей числа в строчки - std::to_string(sigma)
}
//2
int main() {
    try {
        testSomeBlur();
        //testManySigmas();

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

