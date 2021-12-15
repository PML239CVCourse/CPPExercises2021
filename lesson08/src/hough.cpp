#include "hough.h"

#include <libutils/rasserts.h>

double toRadians(double degrees)
{
    const double PI = 3.14159265358979323846264338327950288;
    return degrees * PI / 180.0;
}

double estimateR(double x0, double y0, double theta0radians)
{
    double r0 = x0 * cos(theta0radians) + y0 * sin(theta0radians);
    return r0;
}

cv::Mat buildHough(cv::Mat sobel) {// единственный аргумент - это результат свертки Собелем изначальной картинки
    // проверяем что входная картинка - одноканальная и вещественная:
    rassert(sobel.type() == CV_32FC1, 237128273918006);

    int width = sobel.cols;
    int height = sobel.rows;

    // решаем какое максимальное значение у параметра theta в нашем пространстве прямых
    int max_theta = 360;

    // решаем какое максимальное значение у параметра r в нашем пространстве прямых:
    int max_r = sqrt(width*width+height*height); // TODO замените это число так как вам кажется правильным (отталкиваясь от разрешения картинки - ее ширины и высоты)

    // создаем картинку-аккумулятор, в которой мы будем накапливать суммарные голоса за прямые
    // так же известна как пространство Хафа
    cv::Mat accumulator(max_r, max_theta, CV_32FC1); // TODO подумайте какого разрешения она должна быть и поправьте ее размер

    // TODO не забудьте заполнить эту матрицу-картинку-аккумулятор нулями (очистить)
    for (int i = 0; i< max_r; ++i) {
        for (int j = 0; j< max_theta; j++) {
            accumulator.at<float>(i,j) = 0;
        }
    }

    // проходим по всем пикселям нашей картинки (уже свернутой оператором Собеля)
    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            // смотрим на пиксель с координатами (x0, y0)
            float strength = sobel.at<float>(y0,x0);// TODO считайте его "силу градиента" из картинки sobel

            // теперь для текущего пикселя надо найти все возможные прямые которые через него проходят
            // переберем параметр theta по всему возможному диапазону (в градусах):
            for (int theta0 = 1; theta0 < max_theta; ++theta0) {
                // TODO рассчитайте на базе информации о том какие координаты у пикселя - (x0, y0) и какой параметр theta0 мы сейчас рассматриваем
                // TODO обратите внимание что функции sin/cos принимают углы в радианах, поэтому сначала нужно пересчитать theta0 в радианы (воспользуйтесь константой PI)
                const double PI = 3.14159265358979323846264338327950288;
                double theta = (PI/180)*theta0;
                float r0 = x0*cos(theta)+y0*sin(theta);

                // TODO теперь рассчитайте координаты пикслея в пространстве Хафа (в картинке-аккумуляторе) соответсвующего параметрам theta0, r0
                int i = r0;
                int j = theta0;

                // чтобы проверить не вышли ли мы за пределы картинки-аккумулятора - давайте явно это проверим:
                //rassert(i >= 0, 237891731289044);
                rassert(i < accumulator.rows, 237891731289045);
                rassert(j >= 0, 237891731289046);
                rassert(j < accumulator.cols, 237891731289047);
                // теперь легко отладить случай выхода за пределы картинки
                // TODO просто поставьте точку остановки внутри rassert:
                // нажмите Ctrl+Shift+N -> rasserts.cpp
                // и поставьте точку остановки на 8 строке: "return line;"

                // TODO и добавьте в картинку-аккумулятор наш голос с весом strength (взятый из картинки свернутой Собелем)
                if (i>0) {
                    accumulator.at<float>(i, j) += strength;
                }

            }
        }
    }

    return accumulator;
}

std::vector<PolarLineExtremum> findLocalExtremums(cv::Mat houghSpace)
{
    rassert(houghSpace.type() == CV_32FC1, 234827498237080);

    const int max_theta = 360;
    rassert(houghSpace.cols == max_theta, 233892742893082);
    const int max_r = houghSpace.rows;

    std::vector<PolarLineExtremum> winners;
    float max = 0.0;
    for (int theta = 1; theta < max_theta; ++theta) {
        for (int r = 1; r < max_r; ++r) {
            // TODO
            int i = r;
            int j = theta;
            for (int k = -1; k<=1; k++) {
                for (int m = -1; m <= 1; m++) {
                    if (houghSpace.at<float>(i+k,j+m) > max) {
                        max = houghSpace.at<float>(i+k,j+m);
                    }
                }
            }
            if () {
                PolarLineExtremum line(theta, r, votes);
                winners.push_back(line);
            }
        }
    }
    return winners;
}

std::vector<PolarLineExtremum> filterStrongLines(std::vector<PolarLineExtremum> allLines, double thresholdFromWinner)
{
    std::vector<PolarLineExtremum> strongLines;

    // Эта функция по множеству всех найденных локальных экстремумов (прямых) находит самую популярную прямую
    // и возвращает только вектор из тех прямых, что не сильно ее хуже (набрали хотя бы thresholdFromWinner голосов от победителя, т.е. например половину)

    // TODO

    return strongLines;
}