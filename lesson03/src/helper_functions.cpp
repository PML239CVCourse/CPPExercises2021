#include "helper_functions.h"

#include <libutils/rasserts.h>
#include <iostream>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    for(int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            cv::Vec3b color = image.at<cv::Vec3b>(i,
                                                  j); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];

            // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
            // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

            // пример как заменить цвет по тем же координатам
            if (blue == 0 && green == 0 && red == 0)
                blue = 0xff;
            // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
            // какой пиксель изменился? почему он не чисто красный?
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);

        }
    }
    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // реализуйте функцию которая каждый цвет картинки инвертирует:
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)
    for(int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            cv::Vec3b color = image.at<cv::Vec3b>(i,
                                                  j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(0xff-blue, 0xff-green, 0xff-red);

        }
    }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона

    rassert(object.cols == background.cols, 341241251251351);
    rassert(object.rows == background.rows, 341241251251351);

    for(int i = 0; i < object.rows; i++) {
        for(int j = 0; j < object.cols; j++) {
            cv::Vec3d original = object.at<cv::Vec3b>(i,j);
            if(original[0]+original[1]+original[2] == 0)
                object.at<cv::Vec3b>(i,j) = background.at<cv::Vec3b>(i,j);

        }
    }
    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)

    rassert(object.cols <= largeBackground.cols, 341241251251351);
    rassert(object.rows <= largeBackground.rows, 341241251251351);

    int iBuff = (largeBackground.rows-object.rows) / 2;
    int jBuff = (largeBackground.cols-object.cols) / 2;
    for(int i = 0; i < object.rows; i++) {
        for(int j = 0; j < object.cols; j++) {
            cv::Vec3d original = object.at<cv::Vec3b>(i,j);
            if(original[0]+original[1]+original[2] != 0)
                largeBackground.at<cv::Vec3b>(iBuff+i,jBuff+j) = original;

        }
    }

    return largeBackground;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackgroundNTimes(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)

    rassert(object.cols <= largeBackground.cols, 341241251251351);
    rassert(object.rows <= largeBackground.rows, 341241251251351);

    int N = std::rand()%100;

    for(int n = 0; n < N; n++) {
        int iBuff = rand()%(largeBackground.rows - object.rows);
        int jBuff = rand()%(largeBackground.cols - object.cols);
        for (int i = 0; i < object.rows; i++) {
            for (int j = 0; j < object.cols; j++) {
                cv::Vec3d original = object.at<cv::Vec3b>(i, j);
                if (original[0] + original[1] + original[2] != 0)
                    largeBackground.at<cv::Vec3b>(iBuff + i, jBuff + j) = original;

            }
        }
    }

    return largeBackground;
}

cv::Mat fillBackground(cv::Mat object, cv::Mat largeBackground){
    rassert(object.cols <= largeBackground.cols, 341241251251351);
    rassert(object.rows <= largeBackground.rows, 341241251251351);

    double iK = object.rows*1.0/largeBackground.rows;
    double jK = object.cols*1.0/largeBackground.cols;
    for(int i = 0; i < largeBackground.rows; i++) {
        for(int j = 0; j < largeBackground.cols; j++) {
            cv::Vec3d original = object.at<cv::Vec3b>((int)(i*iK),(int)(j*jK));
            if(original[0]+original[1]+original[2] != 0)
                largeBackground.at<cv::Vec3b>(i,j) = original;

        }
    }

    return largeBackground;
}

cv::Mat doTask4(cv::Mat object, cv::Mat largeBackground, cv::Vec3b color, double dis){

    double iK = largeBackground.rows*1.0/object.rows;
    double jK = largeBackground.cols*1.0/object.cols;
    for(int i = 0; i < object.rows; i++) {
        for(int j = 0; j < object.cols; j++) {
            cv::Vec3d original = object.at<cv::Vec3b>(i,j);
            if(pow(original[0]-color[0], 2)+pow(original[1]-color[1], 2)+pow(original[2]-color[2], 2) <= dis*dis)
                object.at<cv::Vec3b>(i,j) = largeBackground.at<cv::Vec3b>((int)(iK*i), (int)(jK*j));

        }
    }

    return object;
}
