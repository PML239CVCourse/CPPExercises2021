#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hog.h"

#include <opencv2/imgproc.hpp>


void test(std::string name) {
    std::cout << "Processing image " << name << ".jpg..." << std::endl;

    std::string full_path = "lesson10/data/" + name + ".jpg";

    cv::Mat img = cv::imread(full_path);
    rassert(!img.empty(), 238982391080010);

    HoG hog = buildHoG(img);

    std::cout << hog << std::endl;
}


int main() {
    try {
        test("line02");
        test("line01");
        test("line03");
        test("line04");
        test("line11");
        test("line12");
        test("line13");
        test("line14");

        // TODO реализовав функцию distance продолжите эксперименты с буквам в файле mainSymbolsGenerator.cpp (для его запуска надо выбрать lesson10symbolsGenerator)

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

