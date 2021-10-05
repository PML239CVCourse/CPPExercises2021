#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    for (int j = 0; j < image.cols; j++) {
        for (int i = 0; i < image.rows; i++) {
            cv::Vec3b color = image.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0]; // если это число равно 255 - в пикселе много синего, если равно 0 - в пикселе нет синего
            unsigned char green = color[1];
            unsigned char red = color[2];
            if ((blue == 0) && (green == 0) && (red == 0)) {
                color[0] = 255;
            }
            image.at<cv::Vec3b>(j, i) = color;
        }
    }
    // TODO реализуйте функцию которая каждый черный пиксель картинки сделает синим
    return image;
}

cv::Mat invertImageColors(cv::Mat image) {
    // TODO реализуйте функцию которая каждый цвет картинки инвертирует:
    for (int j = 0; j < image.cols; j++) {
        for (int i = 0; i < image.rows; i++) {
            cv::Vec3b color = image.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            color[0] = 255 - blue;
            color[2] = 255 - red;
            color[1] = 255 - green;
            image.at<cv::Vec3b>(j, i) = color;
        }
    }
    return image;
}

cv::Mat addBackgroundInsteadOfBlackPixels(cv::Mat object, cv::Mat background) {
    // TODO реализуйте функцию которая все черные пиксели картинки-объекта заменяет на пиксели с картинки-фона
    rassert(object.cols == background.cols, 341241251251351);
    for (int j = 0; j < background.cols; j++) {
        for (int i = 0; i < background.rows; i++) {
            cv::Vec3b colorO = object.at<cv::Vec3b>(j, i);
            unsigned char blueO = colorO[0];
            unsigned char greenO = colorO[1];
            unsigned char redO = colorO[2];
            cv::Vec3b colorB = background.at<cv::Vec3b>(j, i);
            unsigned char blueB = colorB[0];
            unsigned char greenB = colorB[1];
            unsigned char redB = colorB[2];
            if ((blueO == 0) && (greenO == 0) && (redO == 0)) {
                colorO[0] = blueB;
                colorO[2] = redB;
                colorO[1] = greenB;
                object.at<cv::Vec3b>(j, i) = colorO;
            }
        }
    }

    return object;
}

cv::Mat addBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground) {
    rassert(object.cols < largeBackground.cols, "Object width>Background width");
    rassert(object.rows < largeBackground.rows, "Object height>Background height");
    int startPixW = (largeBackground.cols - object.cols) / 2;
    int startPixH = (largeBackground.rows - object.cols) / 2;
    for (int j = startPixW; j < startPixW + object.cols; j++) {
        for (int i = startPixH; i < startPixH + object.rows; i++) {
            cv::Vec3b colorO = object.at<cv::Vec3b>(j, i);
            unsigned char blueO = colorO[0];
            unsigned char greenO = colorO[1];
            unsigned char redO = colorO[2];
            cv::Vec3b colorB = largeBackground.at<cv::Vec3b>(j, i);
            if ((blueO != 0) || (greenO != 0) || (redO != 0)) {
                colorB[0] = blueO;
                colorB[2] = redO;
                colorB[3] = greenO;
                largeBackground.at<cv::Vec3b>(j, i) = colorB;
            }
        }
    }

    // TODO реализуйте функцию так, чтобы нарисовался объект ровно по центру на данном фоне, при этом черные пиксели объекта не должны быть нарисованы

    return largeBackground;
}

cv::Mat addManyBackgroundInsteadOfBlackPixelsLargeBackground(cv::Mat object, cv::Mat largeBackground, int n) {
    rassert(object.cols < largeBackground.cols, "Object width>Background width");
    rassert(object.rows < largeBackground.rows, "Object height>Background height");
    int endPixW = (largeBackground.cols - object.cols);
    int endPixH = (largeBackground.rows - object.cols);
    for (int a = 0; a < n; a++) {
        int startPixW = (int)rand() / 32767 * endPixW;
        int startPixH = (int)rand() / 327678 * endPixH;
        for (int j = startPixW; j < startPixW + object.cols; j++) {
            for (int i = startPixH; i < startPixH + object.rows; i++) {
                cv::Vec3b colorO = object.at<cv::Vec3b>(j, i);
                unsigned char blueO = colorO[0];
                unsigned char greenO = colorO[1];
                unsigned char redO = colorO[2];
                cv::Vec3b colorB = largeBackground.at<cv::Vec3b>(j, i);
                if ((blueO != 0) || (greenO != 0) || (redO != 0)) {
                    colorB[0] = blueO;
                    colorB[2] = redO;
                    colorB[3] = greenO;
                    largeBackground.at<cv::Vec3b>(j, i) = colorB;
                }
            }
        }
    }
    return largeBackground;
}