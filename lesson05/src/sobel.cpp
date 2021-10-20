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
            // TODO реализуйте усреднение яркости чтобы получить серый цвет
            //  - обратите внимание что если складывать unsigned char - сумма может переполниться, поэтому перед сложением их стоит преобразовать в int или float
            //  - загуглите "RGB to grayscale formula" - окажется что правильно это делать не усреднением в равных пропорциях, а с другими коэффициентами
            float grayIntensity = 0.0f;
            grayIntensity = (float)(0.30*(int)red + 0.59*(int)green + 0.11*(int)blue);
            grayscaleImg.at<float>(j, i) = grayIntensity;
        }
    }

    return grayscaleImg;
}


cv::Mat sobelDXY(cv::Mat img) {
    cv::Mat dxyImg(img.rows, img.cols, CV_32FC2); // в этой картинке мы сохраним обе производные:
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

    // TODO исправьте коээфициенты свертки по вертикальной оси y
    int dySobelKoef[3][3] = {
            {-1,-2,-1},
            { 0, 0, 0},
            { 1, 2, 1},
    };

    // TODO доделайте этот код (в т.ч. производную по оси ty), в нем мы пробегаем по всем пикселям (j,i)
    for (int j = 1; j < img.rows -1; ++j) {
        for (int i = 1; i < img.cols -1; ++i) {
            float dxSum = 0.0f; // судя будем накапливать производную по оси x
            // затем пробегаем по окрестности 3x3 вокруг нашего центрального пикселя (j,i)
            for (int dj = -1; dj <= 1; ++dj) {
                for (int di = -1; di <= 1; ++di) {
                    float intensity = img.at<float>(j + dj, i + di); // берем соседний пиксель из окрестности
                    dxSum += dxSobelKoef[1 + dj][1 + di] * intensity; // добавляем его яркость в производную с учетом веса из ядра Собеля
                }
            }
            float dySum = 0.0f; // судя будем накапливать производную по оси x
            // затем пробегаем по окрестности 3x3 вокруг нашего центрального пикселя (j,i)
            for (int dj = -1; dj <= 1; ++dj) {
                for (int di = -1; di <= 1; ++di) {
                    float intensity = img.at<float>(j + dj, i + di); // берем соседний пиксель из окрестности
                    dySum += dySobelKoef[1 + dj][1 + di] * intensity; // добавляем его яркость в производную с учетом веса из ядра Собеля
                }
            }

            dxyImg.at<cv::Vec2f>(j, i) = cv::Vec2f(dxSum, dySum);
        }
    }
    for(int j = 0; j < img.rows; j++){
        dxyImg.at<cv::Vec2f>(j, 0) = dxyImg.at<cv::Vec2f>(j, 1);
        dxyImg.at<cv::Vec2f>(j, img.cols-1) = dxyImg.at<cv::Vec2f>(j, img.cols-2);
    }
    for(int i = 0; i < img.cols; i++){
        dxyImg.at<cv::Vec2f>(0, i) = dxyImg.at<cv::Vec2f>(1, i);
        dxyImg.at<cv::Vec2f>(img.rows-1, i) = dxyImg.at<cv::Vec2f>(img.rows-2, i);
    }

    return dxyImg; // производная по оси x и оси y (в каждом пикселе два канала - два числа - каждая из компонент производной)
}

cv::Mat convertDXYToDX(cv::Mat img) {
    rassert(img.type() == CV_32FC2,
            238129037129092); // сверяем что в картинке два канала и в каждом - вещественное число
            cv::Mat dxImg(img.rows, img.cols, CV_32FC1); // создаем одноканальную картинку состоящую из 32-битных вещественных чисел
    for (int j = 0; j < img.rows; ++j) {
        for (int i = 0; i < img.cols; ++i) {
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
    cv::Mat dyImg(height, width, CV_32FC1); // создаем одноканальную картинку состоящую из 32-битных вещественных чисел
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec2f dxy = img.at<cv::Vec2f>(j, i);
            float y = std::abs(dxy[1]); // взяли абсолютное значение производной по оси y
            dyImg.at<float>(j, i) = y;
        }
    }
    return dyImg;
}

cv::Mat convertDXYToGradientLength(cv::Mat img) {
    rassert(img.type() == CV_32FC2,
            238129037129092); // сверяем что в картинке два канала и в каждом - вещественное число
    int width = img.cols;
    int height = img.rows;
    cv::Mat Img(height, width, CV_32FC1); // создаем одноканальную картинку состоящую из 32-битных вещественных чисел
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            cv::Vec2f dxy = img.at<cv::Vec2f>(j, i);
            float z = sqrt((dxy[1])*(dxy[1])+(dxy[0])*(dxy[0])); // взяли абсолютное значение производной по оси y
            Img.at<float>(j, i) = z;
        }
    }

    // TODO реализуйте функцию которая считает силу градиента в каждом пикселе
    // точнее - его длину, ведь градиент - это вектор (двухмерный, ведь у него две компоненты), а у вектора всегда есть длина - sqrt(x^2+y^2)
    // TODO и удостоверьтесь что результат выглядит так как вы ожидаете, если нет - спросите меня
    return Img;
}




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