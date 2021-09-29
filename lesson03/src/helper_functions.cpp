#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим
    int width = image.cols; // как в ООП - у картинки есть поля доступные через точку, они называются cols и rows - попробуйте их
    int height = image.rows;
    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    cv::Vec3b color = image.at<cv::Vec3b>(13, 5); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    unsigned char green = color[1];
    unsigned char red = color[2];

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
    blue = 200;
    green = 10;
    red = 10;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            color = image.at<cv::Vec3b>(j, i);
            if(color[0] < 11 && color[1] < 11 && color[2] < 11)
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(blue, green, red);
        }
    }
    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)
    cv::Vec3b color = image.at<cv::Vec3b>(13, 5);
    unsigned char blue = color[0];
    unsigned char green = color[1];
    unsigned char red = color[2];
    int width = image.cols;
    int height = image.rows;
    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            color = image.at<cv::Vec3b>(j, i);
            image.at<cv::Vec3b>(j, i) = cv::Vec3b(255 - color[0], 255 - color[1], 255 - color[2]);
        }
    }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];
    int width = object.cols;
    int height = object.rows;
    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            color = object.at<cv::Vec3b>(j, i);
            if(blue < 11 && green < 11 && red < 11){
                object.at<cv::Vec3b>(j, i) = background.at<cv::Vec3b>(j, i);
            }
        }
    }
    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)
    int owidth = object.cols;
    int oheight = object.rows;
    int bwidth = largeBackground.cols;
    int bheight = largeBackground.rows;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];

    int dj = bheight/2-oheight/2 - 1;
    int di = bwidth/2-owidth/2 - 1;

    for(int j = 0; j < oheight; ++j){
        for(int i = 0; i < owidth; ++i){
            color = object.at<cv::Vec3b>(j, i);
            if(!(blue < 11 && green < 11 && red < 11)){
                largeBackground.at<cv::Vec3b>(j+dj, i+di) = object.at<cv::Vec3b>(j, i);
            }
        }
    }

    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы

    return largeBackground;
}

cv::Mat addNImage(cv::Mat object, cv::Mat largeBackground , int n){
    int owidth = object.cols;
    int oheight = object.rows;
    int bwidth = largeBackground.cols;
    int bheight = largeBackground.rows;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];
    for(int tle = 0; tle < n; ++tle){

        int dj = rand() % (bheight-oheight-5);
        int di = rand() % (bwidth-owidth-5);

        for(int j = 0; j < oheight; ++j){
            for(int i = 0; i < owidth; ++i){
                color = object.at<cv::Vec3b>(j, i);
                if(!(blue < 11 && green < 11 && red < 11)){
                    largeBackground.at<cv::Vec3b>(j+dj, i+di) = object.at<cv::Vec3b>(j, i);
                }
            }
        }
    }
    return largeBackground;
}

cv::Mat NewSize(cv::Mat  object, cv::Mat largeBackground){
    int owidth = object.cols;
    int oheight = object.rows;
    int bwidth = largeBackground.cols;
    int bheight = largeBackground.rows;
    cv::Vec3b color = object.at<cv::Vec3b>(13, 5);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];
    //cv::Scalar tcolor(0, 0, 0);
    //cv::Mat largeObject(bheight, bwidth, CV_8UC3, tcolor);
    for(int j = 0; j < bheight; ++j){
        for(int i = 0; i < bwidth; ++i){
            int tj = j*oheight/bheight;
            int ti = i*owidth/bwidth;
            color = object.at<cv::Vec3b>(tj, ti);
            if(!(blue < 11 && green < 11 && red < 11)){
                largeBackground.at<cv::Vec3b>(j, i) = color;
            }
        }
    }
    return largeBackground;
}

cv::Mat BlackToRand(cv::Mat image){
    cv::Vec3b color = image.at<cv::Vec3b>(0, 0);
    unsigned char &blue = color[0];
    unsigned char &green = color[1];
    unsigned char &red = color[2];
    int width = image.cols;
    int height = image.rows;
    for(int j = 0; j < height; ++j){
        for(int i = 0; i < width; ++i){
            color = image.at<cv::Vec3b>(j, i);
            if((blue < 5 && green < 5 && red < 5)){
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(rand()%256, rand()%256, rand()%256);
            }
        }
    }
    return image;
}

cv::Mat addRedOn(cv::Mat& image, std::vector<std::pair<int,int>> &ClickCoord){
    for(auto& i : ClickCoord){
        image.at<cv::Vec3b>(i.second, i.first) = cv::Vec3b(0, 0, 255);
    }
    return image;
}