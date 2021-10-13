#include "helper_functions.h"

#include <libutils/rasserts.h>


cv::Mat makeAllBlackPixelsBlue(cv::Mat image) {
    for (int j = 0; j < image.cols; j++) {
        for (int i = 0; i < image.rows; i++) {
            cv::Vec3b color = image.at<cv::Vec3b>(i, j);
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
    int startPixW = (int) (largeBackground.cols - object.cols * 1.5) / 2;
    int startPixH = (int) (largeBackground.rows - object.rows * 1.5) / 2;
    for (int j = startPixW; j < startPixW + object.cols; j++) {
        for (int i = startPixH; i < startPixH + object.rows; i++) {
            cv::Vec3b colorO = object.at<cv::Vec3b>(i - startPixH, j - startPixW);
            unsigned char blueO = colorO[0];
            unsigned char greenO = colorO[1];
            unsigned char redO = colorO[2];
            cv::Vec3b colorB = largeBackground.at<cv::Vec3b>(i, j);
            if ((blueO != 0) || (greenO != 0) || (redO != 0)) {
                colorB[0] = blueO;
                colorB[2] = redO;
                colorB[1] = greenO;
                largeBackground.at<cv::Vec3b>(i, j) = colorB;
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
    int endPixH = (largeBackground.rows - object.rows);
    for (int a = 0; a < n; a++) {
        int startPixW = rand() % endPixW;
        int startPixH = rand() % endPixH;
        for (int j = startPixW; j < startPixW + object.cols; j++) {
            for (int i = startPixH; i < startPixH + object.rows; i++) {
                cv::Vec3b colorO = object.at<cv::Vec3b>(i - startPixH, j - startPixW);
                unsigned char blueO = colorO[0];
                unsigned char greenO = colorO[1];
                unsigned char redO = colorO[2];
                cv::Vec3b colorB = largeBackground.at<cv::Vec3b>(i, j);
                if ((blueO != 0) || (greenO != 0) || (redO != 0)) {
                    colorB[0] = blueO;
                    colorB[2] = redO;
                    colorB[1] = greenO;
                    largeBackground.at<cv::Vec3b>(i, j) = colorB;
                }
            }
        }
    }
    return largeBackground;
}

cv::Mat stretch_unicorn_to_castle(cv::Mat background, cv::Mat object) {
    for (int j = 0; j < background.cols; j++) {
        for (int i = 0; i < background.rows; i++) {
            cv::Vec3b colorB = background.at<cv::Vec3b>(i, j);
            int x = (int) (j * object.cols / background.cols);
            int y = (int) (i * object.rows / background.rows);
            cv::Vec3b colorO = object.at<cv::Vec3b>(y, x);
            unsigned char blueO = colorO[0];
            unsigned char greenO = colorO[1];
            unsigned char redO = colorO[2];
            colorB[0] = blueO;
            colorB[2] = redO;
            colorB[1] = greenO;
            background.at<cv::Vec3b>(i, j) = colorB;
        }
    }
    return background;
}

cv::Mat multicolor_unicorn(cv::Mat object) {
    for (int j = 0; j < object.cols; j++) {
        for (int i = 0; i < object.rows; i++) {
            cv::Vec3b color = object.at<cv::Vec3b>(i, j);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            if ((blue == 0) && (green == 0) && (red == 0)) {
                color[0] = rand() % 255;
                color[1] = rand() % 255;
                color[2] = rand() % 255;
                object.at<cv::Vec3b>(i, j) = color;
            }
        }
    }
    return object;
}



cv::Mat replace_pixel_with_transparent(cv::Mat object, cv::Mat background, std::pair<int, int> p, std::vector<std::pair<int, int>> history) {
    cv::Vec3b colorO = object.at<cv::Vec3b>(p.second, p.first);
    unsigned char blueO = colorO[0];
    unsigned char greenO = colorO[1];
    unsigned char redO = colorO[2];
    cv::Vec3b colorB = background.at<cv::Vec3b>(p.second, p.first);
    object.at<cv::Vec3b>(p.second, p.first) = colorB;
    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            cv::Vec3b colorO1 = object.at<cv::Vec3b>(p.second + j, p.first + i);
            unsigned char blueO1 = colorO[0];
            unsigned char greenO1 = colorO[1];
            unsigned char redO1 = colorO[2];
            if ((abs(blueO-blueO1)<30)&&(abs(greenO-greenO1)<30)&&(abs(redO-redO1)<30)){
                history.emplace_back(p.first + i, p.second + j);
            }
        }
    }
    return object;
}
