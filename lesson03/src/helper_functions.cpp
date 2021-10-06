#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим

    // ниже приведен пример как узнать цвет отдельного пикселя - состоящий из тройки чисел BGR (Blue Green Red)
    // чем больше значение одного из трех чисел - тем насыщеннее его оттенок
    // всего их диапазон значений - от 0 до 255 включительно
    // т.е. один байт, поэтому мы используем ниже тип unsigned char - целое однобайтовое неотрицательное число
    //cv::Vec3b color = image.at<cv::Vec3b>(13, 5); // взяли и узнали что за цвет в пикселе в 14-ом ряду (т.к. индексация с нуля) и 6-ой колонке
    //unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
   // unsigned char green = color[1];
   // unsigned char red = color[2];

    // как получить белый цвет? как получить черный цвет? как получить желтый цвет?
    // поэкспериментируйте! например можете всю картинку заполнить каким-то одним цветом

    // пример как заменить цвет по тем же координатам
    //red = 255;
    // запустите эту версию функции и посмотрите на получившуюся картинку - lesson03/resultsData/01_blue_unicorn.jpg
    // какой пиксель изменился? почему он не чисто красный?
    //image.at<cv::Vec3b>(13, 5) = cv::Vec3b(blue, green, red);

    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            if(blue == 0 && red == 0 && green == 0){
                blue = 255;
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
            }
        }
    }



    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    // т.е. пусть ночь станет днем, а сумрак рассеется
    // иначе говоря замените каждое значение яркости x на (255-x) (т.к находится в диапазоне от 0 до 255)

    using namespace std;
    for(int i = 0; i < image.rows; i++){
        for(int j = 0; j < image.cols; j++){
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
            unsigned blue = color[0];
            unsigned green = color[1];
            unsigned red = color[2];
            blue = 255 - blue;
            green = 255 - green;
            red = 255 - red;
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    rassert(object.cols == background.cols, 341241251251351);
    rassert(object.rows == background.rows, 341241251251351);
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    // т.е. что-то вроде накладного фона получится
    using namespace std;
    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color_obj = object.at<cv::Vec3b>(i, j);
            cv::Vec3b color_bg = background.at<cv::Vec3b>(i, j);
            unsigned blue = color_obj[0];
            unsigned green = color_obj[1];
            unsigned red = color_obj[2];
            if(blue == 0 && green == 0 && red == 0){
                blue = color_bg[0];
                green = color_bg[1];
                red = color_bg[2];
            }
            object.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    }


    // гарантируется что размеры картинок совпадают - проверьте это через rassert, вот например сверка ширины:
    rassert(object.cols == background.cols, 341241251251351);
    rassert(object.rows == background.rows, 341241251251351);

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    // теперь вам гарантируется что largeBackground гораздо больше - добавьте проверок этого инварианта (rassert-ов)
    using namespace std;
    rassert(object.cols < largeBackground.cols, 341241251251351);
    rassert(object.rows < largeBackground.rows, 341241251251351);

    int obj_beg_height = (largeBackground.rows - object.rows) / 2;
    int obj_beg_width = (largeBackground.cols - object.cols) / 2;
    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color_obj = object.at<cv::Vec3b>(i, j);
            cv::Vec3b color_bg = largeBackground.at<cv::Vec3b>(obj_beg_height + i, obj_beg_width + j);

            unsigned blue = color_obj[0];
            unsigned green = color_obj[1];
            unsigned red = color_obj[2];

            if(blue == 0 && green == 0 && red == 0){
                blue = color_bg[0];
                green = color_bg[1];
                red = color_bg[2];
            }
            largeBackground.at<cv::Vec3b>(obj_beg_height + i, obj_beg_width + j) = cv::Vec3b(blue, green, red);
        }
    }
    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы

    return largeBackground;
}


cv::Mat addUnicornAtRandomPlace(cv::Mat object, cv::Mat largeBackground){

    using namespace std;
    rassert(object.cols < largeBackground.cols, 341241251251351);
    rassert(object.rows < largeBackground.rows, 341241251251351);

    int obj_beg_height_possible = largeBackground.rows - object.rows;
    int obj_beg_width_possible = largeBackground.cols - object.cols;
    int obj_beg_height = static_cast<int>((std::rand()*1.0*obj_beg_height_possible) / RAND_MAX);
    int obj_beg_width = static_cast<int>((std::rand()*1.0*obj_beg_width_possible) / RAND_MAX);

    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color_obj = object.at<cv::Vec3b>(i, j);
            cv::Vec3b color_bg = largeBackground.at<cv::Vec3b>(obj_beg_height + i, obj_beg_width + j);

            unsigned blue = color_obj[0];
            unsigned green = color_obj[1];
            unsigned red = color_obj[2];

            if(blue == 0 && green == 0 && red == 0){
                blue = color_bg[0];
                green = color_bg[1];
                red = color_bg[2];
            }
            largeBackground.at<cv::Vec3b>(obj_beg_height + i, obj_beg_width + j) = cv::Vec3b(blue, green, red);
        }
    }
    return largeBackground;
}

cv::Mat replaceInRandomColors(cv::Mat object){
    const int max_color_value = 255;
    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color_obj = object.at<cv::Vec3b>(i, j);

            unsigned blue = color_obj[0];
            unsigned green = color_obj[1];
            unsigned red = color_obj[2];

            if(blue == 0 && green == 0 && red == 0){
                blue = static_cast<int>((std::rand()*1.0*max_color_value) / RAND_MAX);
                green = static_cast<int>((std::rand()*1.0*max_color_value) / RAND_MAX);
                red = static_cast<int>((std::rand()*1.0*max_color_value) / RAND_MAX);
            }

            object.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    }
    return object;
}


cv::Mat videoWithBackground(cv::Mat object, cv::Mat init_pict){
    using namespace std;
    //rassert(object.cols > background.cols || object.rows > background.rows, "wrong size of the background");

    for(int i = 0; i < object.rows; i++){
        for(int j = 0; j < object.cols; j++){
            cv::Vec3b color_obj = object.at<cv::Vec3b>(i, j);
            unsigned blue = color_obj[0];
            unsigned green = color_obj[1];
            unsigned red = color_obj[2];

            cv::Vec3b color_obj_init = init_pict.at<cv::Vec3b>(i, j);
            unsigned blue_init = color_obj_init[0];
            unsigned green_init = color_obj_init[1];
            unsigned red_init = color_obj_init[2];

            const int diff_between_colors = 0;

            if((abs((int)blue - (int)blue_init) <= diff_between_colors) && (abs((int)green - (int)green_init) <= diff_between_colors)
            && (abs((int)red - (int)red_init) <= diff_between_colors)){
                green = 255;
                red = 0;
                blue = 0;
            }

            object.at<cv::Vec3b>(i, j) = cv::Vec3b(blue, green, red);
        }
    }
    return object;
}