#include "sobel.h"

#include <libutils/rasserts.h>


cv::Mat convertBGRToGray(cv::Mat img) {
    int height = img.rows;
    int width = img.cols;
    cv::Mat grayscaleImg(height, width, CV_32FC1); // в этой картинке мы сохраним черно-белую версию (оттенки серого)
    // давайте поймем что означает тип картинки CV_32FC1:
    //                                          CV = Computer Vision (библиотека в целом называетсяOpenCV)
    //                                             32F = 32-битное floating число, т.е. вещественное число типа float
    //                                                С1 = 1 channel = один канал - вещественное число описывающее насколько этот пиксель яркий:
    //                                                                                                            (0.0 - черный, 255.0 = белый)

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec3b color = img.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            float grayIntensity = 0.299* (int)red + 0.587*(int)green + 0.114*(int)blue;

            grayscaleImg.at<float>(j, i) = grayIntensity;
        }
    }

    return grayscaleImg;
}


cv::Mat sobelDXY(cv::Mat img) {
    int height = img.rows;
    int width = img.cols;
    cv::Mat dxyImg(height, width, CV_32FC2); // в этой картинке мы сохраним обе производные:
    // давайте поймем что означает тип картинки CV_32FC2:
    //                                          CV = Computer Vision (библиотека в целом называетсяOpenCV)
    //                                             32F = 32-битное floating число, т.е. вещественное число типа float
    //                                                С2 = 2 channels = два канала, т.е. в каждоп пикселе два вещественных числа float
    // их два т.к. мы считаем две производной:
    // - производную по оси x (насколько сильно увеличивается яркость картинки в этом пикселе направо)
    // - производную по оси y (насколько сильно увеличивается яркость картинки в этом пикселе вниз)

    // производную неприятно брать по трем каналам (по трем BGR-цветам),
    // поэтому переданная картинка должна быть черно-белой (оттенки серого)
    // удостоверимся в этом (32-битное вещественное число: 32F + всего 1 канал (channel): C1):
    rassert(img.type() == CV_32FC1, 23781792319049);

    // реализуйте оператор Собеля - заполните dxy
    // https://ru.wikipedia.org/wiki/%D0%9E%D0%BF%D0%B5%D1%80%D0%B0%D1%82%D0%BE%D1%80_%D0%A1%D0%BE%D0%B1%D0%B5%D0%BB%D1%8F

    // это коэффициенты с которыми взвешенно сумиируются яркости соседних пикселей чтобы посчитать производную по оси Х
    int dxSobelKoef[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1},
    };

    int dySobelKoef[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1},
    };

    // TODO доделайте этот код (в т.ч. производную по оси ty), в нем мы пробегаем по всем пикселям (j,i)
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float dxSum = 0.0f; // судя будем накапливать производную по оси x
            float dySum = 0.0f; // судя будем накапливать производную по оси y
            if(i>0&&i<width-1&&j>0&&j<height-1){
                // затем пробегаем по окрестности 3x3 вокруг нашего центрального пикселя (j,i)
                for (int dj = -1; dj <= 1; ++dj) {
                    for (int di = -1; di <= 1; ++di) {
                        float intensity = img.at<float>(j + dj, i + di); // берем соседний пиксель из окрестности
                        dxSum += dxSobelKoef[1 + dj][1 + di] * intensity; // добавляем его яркость в производную с учетом веса из ядра Собеля
                    }
                }
                for (int dj = -1; dj <= 1; ++dj) {
                    for (int di = -1; di <= 1; ++di) {
                        float intensity = img.at<float>(j + dj, i + di); // берем соседний пиксель из окрестности
                        dySum += dySobelKoef[1 + dj][1 + di] * intensity; // добавляем его яркость в производную с учетом веса из ядра Собеля
                    }
                }
            }
            dxyImg.at<cv::Vec2f>(j, i) = cv::Vec2f(dxSum, dySum);
        }
    }

    return dxyImg; // производная по оси x и оси y (в каждом пикселе два канала - два числа - каждая из компонент производной)
}

cv::Mat convertDXYToDX(cv::Mat img) {
    rassert(img.type() == CV_32FC2,
            238129037129092); // сверяем что в картинке два канала и в каждом - вещественное число
    int width = img.cols;
    int height = img.rows;
    cv::Mat dxImg(height, width, CV_32FC1); // создаем одноканальную картинку состоящую из 32-битных вещественных чисел
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec2f dxy = img.at<cv::Vec2f>(j, i);

            float x = std::abs(dxy[0]); // взяли абсолютное значение производной по оси x

            dxImg.at<float>(j, i) = x;
        }
    }
    return dxImg;
}

cv::Mat convertDXYToDY(cv::Mat img) {
    rassert(img.type() == CV_32FC2,
            238129037129092); // сверяем что в картинке два канала и в каждом - вещественное число
    int width = img.cols;
    int height = img.rows;
    cv::Mat dyImg(height, width, CV_32FC1);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec2f dxy = img.at<cv::Vec2f>(j, i);

            float x = std::abs(dxy[1]); // взяли абсолютное значение производной по оси x

            dyImg.at<float>(j, i) = x;
        }
    }
    return dyImg;
}

cv::Mat convertDXYToGradientLength(cv::Mat img) {
    rassert(img.type() == CV_32FC2,
            238129037129092);
    int width = img.cols;
    int height = img.rows;
    cv::Mat gradImg(height, width, CV_32FC1);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec2f pix = img.at<cv::Vec2f>(j, i);

            float L = std::abs(cv::sqrt(pix[0]*pix[0]+pix[1]*pix[1])); // взяли абсолютное значение производной по оси x

            gradImg.at<float>(j, i) = L;
        }
    }
    // точнее - его длину, ведь градиент - это вектор (двухмерный, ведь у него две компоненты), а у вектора всегда есть длина - sqrt(x^2+y^2)
    return gradImg;
}
