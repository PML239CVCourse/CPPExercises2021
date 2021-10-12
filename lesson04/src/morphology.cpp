#include <opencv2/core/mat.hpp>
#include <libutils/rasserts.h>
#include <iostream>
#include "morphology.h"

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


cv::Mat epilepsia(cv::Mat object){
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < object.cols; ++i) {
        for (int j = 0; j < object.rows; j++){
            color = object.at<cv::Vec3b>(j, i);
            if (color[0] == 0 && color[1] == 0 && color[2] == 0){
                object.at<cv::Vec3b>(j, i) = cv::Vec3b(rand(), rand(), rand());
            }
        }
    }
    return object;
}

cv::Mat rast(cv::Mat object, cv::Mat largeBackground) {
    rassert(!object.empty(), 123);
    rassert(!largeBackground.empty(), 321);
    int high = largeBackground.rows;
    int w = largeBackground.cols;
    int high1 = object.rows;
    int w1 = object.cols;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
    double kx = (double)w/(double)w1;
    double ky = (double)high/(double)high1;
//    std::cout << largeBackground.cols << "   " << largeBackground.rows << std::endl;
    for (int i = 0; i < largeBackground.cols; ++i) {
        for (int j = 0; j < largeBackground.rows; j++) {
//            std::cout << i << "   " << j << std::endl;
//            std::cout << (int)((double)i/kx) << "   " << (int)((double)j/kx) << std::endl;
            color1 = largeBackground.at<cv::Vec3b>(j, i);
            color = object.at<cv::Vec3b>((int) ((double) j / ky), (int) ((double) i / kx));
            if (!(color[0] == 0 && color[1] == 0 && color[2] == 0)) {
                largeBackground.at<cv::Vec3b>(j, i) = cv::Vec3b(color[0], color[1], color[2]);
            }
        }
    }
    return largeBackground;
}

cv::Mat baba(cv::Mat largeBackground, cv::Mat object){
    int high = largeBackground.rows;
    int w = largeBackground.cols;
    int high1 = object.rows;
    int w1 = object.cols;
    double kx = (double)w1/(double)w;
    double ky = (double)high1/(double)high;
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    for (int i = 0; i < object.cols; ++i) {
        for (int j = 0; j < object.rows; j++) {
            color1 = largeBackground.at<cv::Vec3b>((int) ((double) j / ky), (int) ((double) i / kx));
            object.at<cv::Vec3b>(j, i) = cv::Vec3b(color1[0], color1[1], color1[2]);
        }
    }
    return object;
}

cv::Mat rast1(cv::Mat object, cv::Mat largeBackground, std::vector<int> pix) {
    rassert(!object.empty(), 123);
    rassert(!largeBackground.empty(), 321);
    rassert(largeBackground.rows == object.rows, 321123);
    rassert(largeBackground.cols == object.cols, 321123);
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);

//    std::cout << pix[0] << " " << pix[1] << " " << pix[2] << "     +-     " << std::endl;
    for (int i = 0; i < largeBackground.cols; ++i) {
        for (int j = 0; j < largeBackground.rows; j++) {
            color1 = largeBackground.at<cv::Vec3b>(j, i);
            color = object.at<cv::Vec3b>(j, i);
//            rassert(color == color1, "ПИЗДА");
            int ret = 15;
//            for (int k = 0; k < pix.size(); ++k) {
            if (((int) color[0] < pix[0]+ret && (int) color[1] < pix[1]+ret && (int) color[2] < pix[2]+ret)&&((int) color[0] > pix[0]-ret && (int) color[1] > pix[1]-ret && (int) color[2] > pix[2]-ret)) {
                object.at<cv::Vec3b>(j, i) = cv::Vec3b(color1[0],color1[1],color1[2]);
            }
//            }
        }
    }
    return object;
}

cv::Mat rast2(cv::Mat object, cv::Mat largeBackground, cv::Mat fon) {
    rassert(!object.empty(), 123);
    rassert(!largeBackground.empty(), 321);
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    cv::Vec3b color1 = largeBackground.at<cv::Vec3b>(13, 5);
    cv::Vec3b colorf = fon.at<cv::Vec3b>(13, 5);
    //std::cout << pix[0] << "     +-     " << std::endl;
    for (int i = 0; i < largeBackground.cols; ++i) {
        for (int j = 0; j < largeBackground.rows; j++) {
            color1 = largeBackground.at<cv::Vec3b>(j, i);
            color = object.at<cv::Vec3b>(j, i);
            colorf = fon.at<cv::Vec3b>(j,i);
            int ret = 25;
//            for (int k = 0; k < pix.size(); ++k) {
            if (((int) color[0] < (int)colorf[0]+ret && (int) color[1] < (int)colorf[1]+ret && (int) color[2] < (int)colorf[2]+ret)&&((int) color[0] > (int)colorf[0]-ret && (int) color[1] > (int)colorf[1]-ret && (int) color[2] > (int)colorf[2]-ret)) {
                object.at<cv::Vec3b>(j, i) = cv::Vec3b(color1[0], color1[1], color1[2]);
            }
//            }
        }
    }
    return object;
}

std::vector<std::vector<int>> Dilate(std::vector<std::vector<int>> mas, int r){
    std::vector<std::vector<int>> a = mas;
    rassert(a.size() == mas.size(), 123123)
    rassert(a[0].size() == mas[0].size(), 123123)
    int w1 = -r, w2 = r, h1 = -r, h2 = r;
    for (int i = 0; i < mas.size(); ++i) {
        for (int j = 0; j < mas[i].size(); j++) {
            if (mas[i][j] == 1){
                if (i+w1 <= 0){
                    w1 = -i;
                }
                if(i+w2 >= mas.size()-1){
                    w2 = mas.size()-1 - i;
                }
                if(j+h1 <= 0){
                    h1 = -j;
                }
                if(j+h2 >= mas[i].size()-1){
                    h2 = mas[i].size()-1 - j;
                }
                for (int k = w1; k <= w2; ++k) {
                    for (int l = h1; l <= h2; ++l) {
                        a[i+k][j+l] = 1;
                    }
                }
            }
        }
    }
    return a;
}

std::vector<std::vector<int>> Erode(std::vector<std::vector<int>> mas, int r){
    std::vector<std::vector<int>> a = mas;
    rassert(a.size() == mas.size(), 123123)
    rassert(a[0].size() == mas[0].size(), 123123)
    int w1 = -r, w2 = r, h1 = -r, h2 = r;
    for (int i = 0; i < mas.size(); ++i) {
        for (int j = 0; j < mas[i].size(); j++) {
            if (mas[i][j] == 0){
                if (i+w1 <= 0){
                    w1 = -i;
                }
                if(i+w2 >= mas.size()-1){
                    w2 = mas.size()-1 - i;
                }
                if(j+h1 <= 0){
                    h1 = -j;
                }
                if(j+h2 >= mas[i].size()-1){
                    h2 = mas[i].size()-1 - j;
                }
                for (int k = w1; k <= w2; ++k) {
                    for (int l = h1; l <= h2; ++l) {
                        a[i+k][j+l] = 0;
                    }
                }
            }
        }
    }
    return a;
}