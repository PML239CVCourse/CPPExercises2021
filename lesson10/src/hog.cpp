#include "hog.h"

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


HoG buildHoG(cv::Mat grad_x, cv::Mat grad_y) {
    rassert(grad_x.type() == CV_32FC1, 2378274892374008);
    rassert(grad_y.type() == CV_32FC1, 2378274892374008);

    rassert(grad_x.rows == grad_y.rows, 3748247980010);
    rassert(grad_x.cols == grad_y.cols, 3748247980011);
    int height = grad_x.rows;
    int width = grad_x.cols;

    HoG hog(NBINS, 0);

    // 1) увеличьте размер вектора hog до NBINS (ведь внутри это просто обычный вектор вещественных чисел)
    // 2) заполните его нулями
    // 3) пробегите по всем пикселям входной картинки и посмотрите на каждый градиент
    // (определенный двумя числами: dx проекцией на ось x в grad_x, dy проекцией на ось y в grad_y)
    // 4) определите его силу (корень из суммы квадратов), определите его угол направления:
    // рекомендую воспользоваться atan2(dy, dx) - он возвращает радианы - https://en.cppreference.com/w/cpp/numeric/math/atan2
    // прочитайте по ссылке на документацию (в прошлой строке) - какой диапазон значений у угла-результата atan2 может быть?
    // 5) внесите его силу как голос за соответствующую его углу корзину
    float pi = 3.14159265;
    float eps = 0.0000001;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double dx = grad_x.at<float>(j, i);
            double dy = grad_y.at<float>(j, i);
            double strength = sqrt(dx * dx + dy * dy);
            int bin = floor(((atan2(dy, dx)+pi)/(2*pi)-eps)*NBINS);
            rassert(bin >= 0, 3842934728039);
            rassert(bin < NBINS, 34729357289040);
            hog[bin] += strength;
        }
    }
    double sum = 0;
    for (double f:hog)
        sum+=f;
    for (double &f:hog)
        f/=sum;
    rassert(hog.size() == NBINS, 23478937290010);
    return hog;
}

// Эта функция просто преобразует картинку в черно-белую, строит градиенты и вызывает buildHoG объявленный выше
HoG buildHoG(cv::Mat originalImg) {
    cv::Mat img = originalImg.clone();
    rassert(img.type() == CV_8UC3, 347283678950077);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого
    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    HoG hog = buildHoG(grad_x, grad_y);
    return hog;
}

// Пример корректного вывода (выводите не само значение накопленных голосов за каждое направление, а процент от общей суммы голосов):
// HoG[22.5=0%, 67.5=78%, 112.5=21%, 157.5=0%, 202.5=0%, 247.5=0%, 292.5=0%, 337.5=0%]
std::ostream &operator<<(std::ostream &os, const HoG &hog) {
    rassert(hog.size() == NBINS, 234728497230016);

    os << "HoG[";
    for (int bin = 0; bin < NBINS; ++bin) {
        double angleInDegrees = (360.0/NBINS)*bin+(360.0/NBINS/2.0);
        double percentage = hog[bin];
        os << (double)((int)(angleInDegrees * 10 + 0.5))/10.0 << "=" << (double)((int)(percentage * 1000 + 0.5))/10.0 << "%, ";
    }
    os << "]";
    return os;
}


double distance(HoG a, HoG b) {
    rassert(a.size() == NBINS, 237281947230077);
    rassert(b.size() == NBINS, 237281947230078);
    double res = 0.0;
    for(int i = 0; i<NBINS; i++){
        res+=(a[i]-b[i])*(a[i]-b[i]);
    }
    res = sqrt(res);
    // подумайте - как можно добавить независимость (инвариантность) гистаграммы градиентов к тому насколько контрастная или блеклая картинка?
    // подсказка: на контрастной картинке все градиенты гораздо сильнее, а на блеклой картинке все градиенты гораздо слабее, но пропорции между градиентами (распроцентовка) не изменны!


    return res;
}
