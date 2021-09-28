#include "helper_functions.h"

#include <libutils/rasserts.h>
#include <random>
#include <iostream>
using namespace std;
using namespace cv;
cv::Mat getanime(cv::Mat object, cv::Mat largeBackground,int n)
{
    rassert(largeBackground.rows>object.rows,"ti tupoi schto li?");
    rassert(largeBackground.cols>object.cols,"ti tupoi schto li?");
    for(int cyc=0;cyc<n;++cyc)
    {
        int ox=largeBackground.rows-object.rows;
        int oy=largeBackground.cols-object.cols;
        int x=rand()%(ox+1);
        int y=rand()%(oy+1);
        for(int i=0;i<object.rows;++i)
        {
            for(int j=0;j<object.cols;++j)
            {
                cv::Vec3b color = object.at<cv::Vec3b>(i, j);
                cv::Vec3b &color2 = largeBackground.at<cv::Vec3b>(x+i, y+j);
                unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
                unsigned char green = color[1];
                unsigned char red = color[2];
                if((blue+green+red)>=100)
                {
                    color2[0]=blue;color2[1]=green;color2[2]=red;
                }
            }
        }
    }
    return largeBackground;
}
cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((abs(blue+green+red)>255*3-150))
            {
                blue=255;green=0;red=0;
            }
            color={blue,green,red};
        }
    }
    return image;
    //  реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    cv::Vec3b color = image.at<cv::Vec3b>(13, 5); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    //unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
    //unsigned char green = color[1];
    //unsigned char red = color[2];

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
    //red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
    //image.at<cv::Vec3b>(13, 5) = cv::Vec3b(blue, green, red);

    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // реализуйте функцию которая каждый цвет картинки инвертирует:
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            blue=255-blue;green=255-green;red=255-red;
            color={blue,green,red};
        }
    }
    return image;
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    for(int i=0;i<object.rows;++i)
    {
        for(int j=0;j<object.cols;++j)
        {
            cv::Vec3b &color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b color2 = background.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((blue+green+red)<100)
            {
                blue=color2[0];green=color2[1];red=color2[2];
            }
            color={blue,green,red};
        }
    }
    return object;
    // т.е. что-то вроде накладного фона получится

    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    rassert(largeBackground.rows>object.rows,"ti tupoi schto li?");
    rassert(largeBackground.cols>object.cols,"ti tupoi schto li?");
    int x=(largeBackground.rows-object.rows)/2;
    int y=(largeBackground.cols-object.cols)/2;
    for(int i=0;i<object.rows;++i)
    {
        for(int j=0;j<object.cols;++j)
        {
            cv::Vec3b color = object.at<cv::Vec3b>(i, j);
            cv::Vec3b &color2 = largeBackground.at<cv::Vec3b>(x+i, y+j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((blue+green+red)>=100)
            {
                color2[0]=blue;color2[1]=green;color2[2]=red;
            }
        }
    }
    return largeBackground;

    // реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы
}
cv::Mat blackrandom(cv::Mat image1) {
    cv::Mat image=image1.clone();
    std::cout<<time(NULL)+getpid()+rand()<<std::endl;
    srand(time(NULL)+getpid()+rand());
    mt19937 rnd(time(NULL)+getpid()+rand());
    for(int i=0;i<image.rows;++i)
    {
        for(int j=0;j<image.cols;++j)
        {
            cv::Vec3b &color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if((abs(blue+green+red)<150))
            {
                blue=rnd()%256;green=rnd()%256;red=rnd()%256;
            }
            color={blue,green,red};
        }
    }
    return image;
}