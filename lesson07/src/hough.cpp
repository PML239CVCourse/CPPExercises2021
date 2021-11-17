#include <iostream>
#include "hough.h"

#include <libutils/rasserts.h>
using namespace std;
cv::Mat buildHough(cv::Mat sobel) {// единственный аргумент - это результат свертки Собелем изначальной картинки
    // проверяем что входная картинка - одноканальная и вещественная:
    rassert(sobel.type() == CV_32FC1, 237128273918006);

    int width = sobel.cols;
    int height = sobel.rows;
    cout<<width<<' '<<height<<endl;
    // решаем какое максимальное значение у параметра theta в нашем пространстве прямых
    int max_theta = 360;

    // решаем какое максимальное значение у параметра r в нашем пространстве прямых:
    int max_r = 100; //  замените это число так как вам кажется правильным (отталкиваясь от разрешения картинки - ее ширины и высоты)

    // создаем картинку-аккумулятор, в которой мы будем накапливать суммарные голоса за прямые
    // так же известна как пространство Хафа
    cv::Mat accumulator(max_r, max_theta, CV_32FC1,0.0f); //  подумайте какого разрешения она должна быть и поправьте ее размер
    for(int i=0;i<max_r;++i)
    {
        for(int j=0;j<max_theta;++j)
        {
            accumulator.at<float>(i, j) = 0;
        }
    }
    //  не забудьте заполнить эту матрицу-картинку-аккумулятор нулями (очистить)

    // проходим по всем пикселям нашей картинки (уже свернутой оператором Собеля)
    double z=sqrt(width*width+height*height+0.0);
    for (int y0 = 0; y0 < height; ++y0) {
        for (int x0 = 0; x0 < width; ++x0) {
            // смотрим на пиксель с координатами (x0, y0)
            float strength = sobel.at<float>(y0, x0);//  считайте его "силу градиента" из картинки sobel
            // теперь для текущего пикселя надо найти все возможные прямые которые через него проходят
            // переберем параметр theta по всему возможному диапазону (в градусах):
            if(strength>1e10)
            {
                cout<<x0<<' '<<y0<<endl;
            }
            for (int theta0 = 0; theta0 < max_theta; ++theta0) {
                //  рассчитайте на базе информации о том какие координаты у пикселя - (x0, y0) и какой параметр theta0 мы сейчас рассматриваем
                //  обратите внимание что функции sin/cos принимают углы в радианах, поэтому сначала нужно пересчитать theta0 в радианы (воспользуйтесь константой PI)
                const double PI = 3.14159265358979323846264338327950288;
                double th=(theta0+0.0)/(max_theta+0.0)*2*PI;
                float r0 = x0*cos(th)+y0*sin(th);
                //  теперь рассчитайте координаты пикслея в пространстве Хафа (в картинке-аккумуляторе) соответсвующего параметрам theta0, r0
                int i = theta0;
                int j = ((r0+0.0)/z)*max_r;
                if(j>=max_r || j<0) continue;

                // чтобы проверить не вышли ли мы за пределы картинки-аккумулятора - давайте явно это проверим:
                rassert(i >= 0, 237891731289044);
                rassert(i < accumulator.cols, 237891731289045);
                rassert(j >= 0, 237891731289046);
                rassert(j < accumulator.rows, 237891731289047);
                // теперь легко отладить случай выхода за пределы картинки
                //  просто поставьте точку остановки внутри rassert:
                // нажмите Ctrl+Shift+N -> rasserts.cpp
                // и поставьте точку остановки на 8 строке: "return line;"

                //  и добавьте в картинку-аккумулятор наш голос с весом strength (взятый из картинки свернутой Собелем)
                accumulator.at<float>(j, i) += strength;
            }
        }
    }
    int size1;
    size1=4;
    int k;
    int size;
    int size2;
    size2=size1;
    size= size1;
    for(int i0=0;i0<accumulator.cols;i0++)
    {
        for (int j0 = 0; j0 < accumulator.rows ; j0++)
        { k=0;
            size=min(size1,min(i0,accumulator.cols-i0));
            size2=min(size1,min(j0,accumulator.rows -j0));
            if(size>0) {
                for (int t0 = -size2; t0 < size2; t0++) {
                    for (int r0 = -size; r0 < size; r0++) {
                        if (accumulator.at<float>(j0, i0) < accumulator.at<float>(j0 + t0, i0 + r0))
                            k++;
                    }

                }
                if (k > 0)
                    accumulator.at<float>(j0, i0) = 0;
            }

        }
    }
    return accumulator;
}