#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим

    cv::Vec3b color = image.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < image.cols; ++i) {
        for (int j = 0; j < image.rows; j++){
            color = image.at<cv::Vec3b>(j, i);
            if (color[0] == 0 && color[1] == 0 && color[2] == 0){
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 0, 0);
            }
        }
    }
    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
     // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    unsigned char green = color[1];
    unsigned char red = color[2];

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
//    red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
//    image.at<cv::Vec3b>(13, 5) = cv::Vec3b(blue, green, red);

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)
    cv::Vec3b color = image.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < image.cols; ++i) {
        for (int j = 0; j < image.rows; j++){
            color = image.at<cv::Vec3b>(j, i);
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(255-color[0], 255-color[1], 255-color[2]);
        }
    }

    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится

    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);
    rassert(object.rows == background.rows, 334653465);
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = background.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < object.cols; ++i) {
        for (int j = 0; j < object.rows; j++){
            color = object.at<cv::Vec3b>(j, i);
            color1 = background.at<cv::Vec3b>(j, i);
            if (color[0] == 0 && color[1] == 0 && color[2] == 0){
                object.at<cv::Vec3b>(j, i) = cv::Vec3b(color1[0], color1[1], color1[2]);
            }
        }
    }

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)

    int high = largeBackground.rows;
    int w = largeBackground.cols;
    int high1 = object.rows;
    int w1 = object.cols;
    int x0 = (w-w1)/2;
    int y0 = (high-high1)/2;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < largeBackground.cols; ++i) {
        for (int j = 0; j < largeBackground.rows; j++){
            color1 = largeBackground.at<cv::Vec3b>(j, i);
            if (i > x0 && i < x0+w1 && j > y0 && j < y0+high1){
                color = object.at<cv::Vec3b>(j-y0, i-x0);
                if (!(color[0] == 0 && color[1] == 0 && color[2] == 0)){
                    largeBackground.at<cv::Vec3b>(j, i) = cv::Vec3b(color[0], color[1], color[2]);
                }
            }
        }
    }
    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы



    return largeBackground;
}

cv::Mat sosibibu(cv::Mat object, cv::Mat largeBackground) {
    int high = largeBackground.rows;
    int w = largeBackground.cols;
    int high1 = object.rows;
    int w1 = object.cols;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
    int N = rand();
    for (int q = 0; q < N; ++q) {
        int x0 = rand()%(w-w1);
        int y0 = rand()%(high-high1);
        for (int i = 0; i < largeBackground.cols; ++i) {
            for (int j = 0; j < largeBackground.rows; j++){
                color1 = largeBackground.at<cv::Vec3b>(j, i);
                if (i > x0 && i < x0+w1 && j > y0 && j < y0+high1){
                    color = object.at<cv::Vec3b>(j-y0, i-x0);
                    if (!(color[0] == 0 && color[1] == 0 && color[2] == 0)){
                        largeBackground.at<cv::Vec3b>(j, i) = cv::Vec3b(color[0], color[1], color[2]);
                    }
                }
            }
        }
    }
    return largeBackground;
}

cv::Mat rast(cv::Mat object, cv::Mat largeBackground) {
    int high = largeBackground.rows;
    int w = largeBackground.cols;
    int high1 = object.rows;
    int w1 = object.cols;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
        int x0 = rand()%(w-w1);
        int y0 = rand()%(high-high1);
        for (int i = 0; i < largeBackground.cols; ++i) {
            for (int j = 0; j < largeBackground.rows; j++){
                color1 = largeBackground.at<cv::Vec3b>(j, i);
                if (i > x0 && i < x0+w1 && j > y0 && j < y0+high1){
                    color = object.at<cv::Vec3b>(j-y0, i-x0);
                    if (!(color[0] == 0 && color[1] == 0 && color[2] == 0)){
                        largeBackground.at<cv::Vec3b>(j, i) = cv::Vec3b(color[0], color[1], color[2]);
                    }
                }
            }
        }
    }
    return largeBackground;
}
