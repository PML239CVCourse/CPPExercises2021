#include "helper_functions.h"
#include <iostream>
#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    int width = image.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = image.rows;
    for(int i = 0;i<height;i++)
        for(int j =0;j<width;j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];

            // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
            // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

            // пример как заменить цвет по тем же координатам
            if(blue<=10&&green<=10&&red<=10){
                red = 0;
                green = 0;
                blue = 255;
            }
            // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
            // какой пиксель изменился? почему он не чисто красный?
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }


    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    int width = image.cols;
    int height = image.rows;
    for(int i = 0;i<height;i++)
        for(int j =0;j<width;j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = 255-color[0];
            unsigned char green = 255-color[1];
            unsigned char red = 255-color[2];
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    rassert(object.cols==background.cols,341241251251351);
    rassert(object.rows==background.rows, 95871698716);
    int width = object.cols;
    int height = object.rows;
    for(int i = 0;i<height;i++)
        for(int j =0;j<width;j++){
            cv::Vec3b object_color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b background_color = background.at<cv::Vec3b>(i, j);
            unsigned char blue = object_color[0];
            unsigned char green = object_color[1];
            unsigned char red = object_color[2];
            if(blue<=10&&green<=10&&red<=10){
                red = background_color[2];
                green = background_color[1];
                blue = background_color[0];
            }
            object.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)
//    int width = largeBackground.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
//    int height = largeBackground.rows;
//    std::cout << "Unicorn image loaded: " << width << "x" << height << std::endl;
    rassert(object.cols<largeBackground.cols,9341251351);
    rassert(object.rows<largeBackground.rows, 995871616);
    int object_width = object.cols;
    int object_height = object.rows;
    int largeBackground_width = largeBackground.cols;
    int largeBackground_height = largeBackground.rows;
    int minus_i = (largeBackground_height-object_height)/2;
    int minus_j = (largeBackground_width-object_width)/2;
    for(int i = 0;i<object_height;i++)
        for(int j =0;j<object_width;j++){
            cv::Vec3b object_color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b background_color = largeBackground.at<cv::Vec3b>(i+minus_i, j+minus_j);
            unsigned char blue = object_color[0];
            unsigned char green = object_color[1];
            unsigned char red = object_color[2];
            if(blue<=10&&green<=10&&red<=10){
                red = background_color[2];
                green = background_color[1];
                blue = background_color[0];
            }
            largeBackground.at<cv::Vec3b>(i+minus_i, j+minus_j) = cv::Vec3b(blue, green, red);
        }

    return largeBackground;
}

cv::Mat generateRandomInserts(cv::Mat object, cv::Mat largeBackground) {
    rassert(object.cols<largeBackground.cols,9341251351);
    rassert(object.rows<largeBackground.rows, 995871616);
    int object_width = object.cols;
    int object_height = object.rows;
    int largeBackground_width = largeBackground.cols;
    int largeBackground_height = largeBackground.rows;
    for(int cnt = 0;cnt<100;cnt++){
        int minus_i = rand()%(largeBackground_height-object_height);
        int minus_j = rand()%(largeBackground_width-object_width);
        for(int i = 0;i<object_height;i++)
            for(int j =0;j<object_width;j++){
                cv::Vec3b object_color = object.at<cv::Vec3b>(i, j);
                cv::Vec3b background_color = largeBackground.at<cv::Vec3b>(i+minus_i, j+minus_j);
                unsigned char blue = object_color[0];
                unsigned char green = object_color[1];
                unsigned char red = object_color[2];
                if(blue<=10&&green<=10&&red<=10){
                    red = background_color[2];
                    green = background_color[1];
                    blue = background_color[0];
                }
                largeBackground.at<cv::Vec3b>(i+minus_i, j+minus_j) = cv::Vec3b(blue, green, red);
            }
    }
    return largeBackground;
}