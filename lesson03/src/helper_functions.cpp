#include "helper_functions.h"

#include <libutils/rasserts.h>
#include <iostream>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число

    cv::Vec3b color = image.at<cv::Vec3b>(13, 5); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    unsigned char green = color[1];
    unsigned char red = color[2];

    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
             blue = color[0];
             green = color[1];
             red = color[2];
             if (blue == 0 && red == 0 && green == 0) image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 0, 0);
        }
    }


    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
    red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
    image.at<cv::Vec3b>(13, 5) = cv::Vec3b(blue, green, red);

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    unsigned char blue;
    unsigned char green;
    unsigned char red;

    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            blue = color[0];
            green = color[1];
            red = color[2];
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(255-blue, 255 - green, 255 - red);
        }
    }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится
    unsigned char blue; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    unsigned char green;
    unsigned char red;

    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color = object.at<cv::Vec3b>(i, j);
            blue = color[0];
            green = color[1];
            red = color[2];
            if (blue == 0 && red == 0 && green == 0) object.at<cv::Vec3b>(i, j) = background.at<cv::Vec3b>(i, j);
        }
    }
    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)

    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color = object.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            if (!(blue == 0 && red == 0 && green == 0)) {
                int y = (largeBackground.rows-object.rows)/2-1+i;
                int x = (largeBackground.cols-object.cols)/2-1+j;
                rassert(x < largeBackground.cols, "x out of diapason");
                rassert(y < largeBackground.rows, "y out of diapason");
                largeBackground.at<cv::Vec3b>(y, x) = object.at<cv::Vec3b>(i, j);
            }
        }
    }

    return largeBackground;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackgroundNtimes(cv::Mat object, cv::Mat largeBackground, int n){
        for(int m = 0; m < n; m++){
            int x = rand() % (largeBackground.cols - object.cols - 1);
            int y = rand() % largeBackground.rows - object.rows - 1;
            for(int i = 0; i < object.rows; i++){
                for(int j = 0; j < object.cols; j++){
                    cv::Vec3b color = object.at<cv::Vec3b>(i, j);
                    unsigned char blue = color[0];
                    unsigned char green = color[1];
                    unsigned char red = color[2];

                    if (!(blue == 0 && red == 0 && green == 0)) {
                        //std::cout << i << "  " << j << std::endl;
                        rassert(x+j < largeBackground.cols, "x out of diapason");
                        rassert(y+i < largeBackground.rows, "y out of diapason");
                        largeBackground.at<cv::Vec3b>(y+i, x+j) = object.at<cv::Vec3b>(i, j);
                    }
                }
            }
        }
        return largeBackground;
    }

cv::Mat makeAllBlackPixelsRandom(cv::Mat image){

    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            if (blue == 0 && red == 0 && green == 0) image.at<cv::Vec3b>(i, j) = cv::Vec3b(rand() % 255, rand() % 255, rand() % 255);
        }
    }
    return image;
}

cv::Mat makePixelsRed(std::vector<int> vectorX, std::vector<int> vectorY, cv::Mat frame){
    for(int i = 0; i < vectorX.size(); i++){
        frame.at<cv::Vec3b>(vectorY[i], vectorX[i]) = cv::Vec3b (1, 1, 254);
    }
    return frame;
}