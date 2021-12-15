#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "hog.h"

#include <opencv2/imgproc.hpp>


#define NSAMPLES_PER_LETTER 5
#define LETTER_DIR_PATH std::string("lesson10/generatedData/letters")


int randFont() {
    int fonts[] = {
//            cv::FONT_HERSHEY_SIMPLEX,
//            cv::FONT_HERSHEY_PLAIN,
//            cv::FONT_HERSHEY_DUPLEX,
//            cv::FONT_HERSHEY_COMPLEX,
//            cv::FONT_HERSHEY_TRIPLEX,
            cv::FONT_HERSHEY_COMPLEX_SMALL,
//            cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
//            cv::FONT_HERSHEY_SCRIPT_COMPLEX,
    };
    // Выбираем случайный шрифт из тех что есть в OpenCV
    int nfonts = (sizeof(fonts) / sizeof(int));
    int font = rand() % nfonts;

    // С вероятностью 20% делаем шрифт наклонным (italic)
//    bool is_italic = ((rand() % 5) == 0);
//    if  (is_italic) {
//        font = font | cv::FONT_ITALIC;
//    }

    return font;
}

double randFontScale() {
    double min_scale = 2.5;
    double max_scale = 3.0;
    double scale = min_scale + (max_scale - min_scale) * ((rand() % 100) / 100.0);
    return scale;
}

int randThickness() {
    int min_thickness = 2;
    int max_thickness = 3;
    int thickness = min_thickness + rand() % (max_thickness - min_thickness + 1);
    return thickness;
}

cv::Scalar randColor() {
    return cv::Scalar(rand() % 128, rand() % 128, rand() % 128); // можно было бы брать по модулю 255, но так цвета будут темнее и контрастнее
}

cv::Mat generateImage(std::string text, int width=128, int height=128) {
    cv::Scalar white(255, 255, 255);
    cv::Scalar backgroundColor = white;
    // Создаем картинку на которую мы нанесем символ (пока что это просто белый фон)
    cv::Mat img(height, width, CV_8UC3, backgroundColor);

    // Выберем случайные параметры отрисовки текста - шрифт, размер, толщину, цвет
    int font = randFont();
    double fontScale = randFontScale();
    int thickness = randThickness();
    cv::Scalar color = randColor();

    // Узнаем размер текста в пикселях (если его нарисовать с указанными параметрами)
    int baseline = 0;
    cv::Size textPixelSize = cv::getTextSize(text, font, fontScale, thickness, &baseline);

    // Рисуем этот текст идеально в середине картинки
    // (для этого и нужно было узнать размер текста в пикселях - чтобы сделать отступ от середины картинки)
    // (ведь при рисовании мы указываем координаты левого нижнего угла текста)
    int xLeft = (width / 2) - (textPixelSize.width / 2);
    int yBottom = (height / 2) + (textPixelSize.height / 2);
    cv::Point coordsOfLeftBorromCorner(xLeft, yBottom);
    cv::putText(img, text, coordsOfLeftBorromCorner, font, fontScale, color, thickness);

    return img;
}

void generateAllLetters() {
    srand(239017); // фиксируем зерно генератора случайных чисел (чтобы картинки от раза к разу генерировались с одинаковыми шрифтами, размерами и т.п.)

    for (char letter = 'a'; letter <= 'z'; ++letter) {

        // Создаем папку для текущей буквы:
        std::string letterDir = LETTER_DIR_PATH + "/" + letter;
        std::filesystem::create_directory(letterDir);

        for (int sample = 1; sample <= NSAMPLES_PER_LETTER; ++sample) {
            std::string text = std::string("") + letter;
            cv::Mat img = generateImage(text);

            cv::blur(img, img, cv::Size(3, 3));

            std::string letterSamplePath = letterDir + "/" + std::to_string(sample) + ".png";
            cv::imwrite(letterSamplePath, img);
        }
    }
}


void experiment1() {
    // TODO Проведите эксперимент 1:
    // Пробежав в цикле по каждой букве - посчитайте насколько сильно она отличается между своими пятью примерами? (NSAMPLES_PER_LETTER)
    // Для каждой буквы выведите:
    // 1) Среднее попарное расстояние (среднюю похожесть) между всеми примерами этой буквы
    // 2) Максимальное попарное расстояние между примерами этой буквы
    //
    // А так же среди всех максимальных расстояний найдите максимальное и выведите его в конце

    std::cout << "________Experiment 1________" << std::endl;
    for (char letter = 'a'; letter <= 'z'; ++letter) {
        std::string letterDir = LETTER_DIR_PATH + "/" + letter;

        for (int sampleA = 1; sampleA <= NSAMPLES_PER_LETTER; ++sampleA) {
            for (int sampleB = sampleA + 1; sampleB <= NSAMPLES_PER_LETTER; ++sampleB) {
                cv::Mat a = cv::imread(letterDir + "/" + std::to_string(sampleA) + ".png");
                cv::Mat b = cv::imread(letterDir + "/" + std::to_string(sampleB) + ".png");
                HoG hogA = buildHoG(a);
                // TODO
            }
        }
//        std::cout << "Letter " << letter << ": max=" << distMax << ", avg=" << (distSum / distN) << std::endl;
    }
}

void experiment2() {
    // TODO Проведите эксперимент 2:
    // Для каждой буквы найдите среди остальных наиболее похожую и наименее похожую
    //
    // А так же среди всех минимальных расстояний найдите среднее и выведите его в конце
    //  - Посмотрите и подумайте: как это число соотносится с максимальным расстоянием из прошлого эксперимента?
    //  - Какие буквы невозможно различить закодировав их в HoG?
    //  - Можно ли с этим что-то сделать?

    std::cout << "________Experiment 2________" << std::endl;
    for (char letterA = 'a'; letterA <= 'z'; ++letterA) {
        std::string letterDirA = LETTER_DIR_PATH + "/" + letterA;

        for (char letterB = 'a'; letterB <= 'z'; ++letterB) {
            if (letterA == letterB) continue;

            // TODO
        }

//        std::cout << "Letter " << letterA << ": max=" << letterMax << "/" << distMax << ", min=" << letterMin << "/" << distMin << std::endl;
    }
}


int main() {
    try {
        std::cout << "Generating letters images..." << std::endl;

        generateAllLetters();

        std::cout << "Images with letters were generated!" << std::endl;

        // TODO:
        experiment1();

        // TODO:
        experiment2();

    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

