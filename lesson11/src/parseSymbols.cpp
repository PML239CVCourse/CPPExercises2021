#include "parseSymbols.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>

#include <libutils/rasserts.h>

#include <opencv2/imgproc.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#define NBINS 8
typedef std::vector<double> HoG;
#define NSAMPLES_PER_LETTER 5
#define LETTER_DIR_PATH std::string("lesson11/resultsData/letters")
typedef HoG hogt;
using namespace std;
using namespace cv;
const double pi=3.14159265358979;
void cou(HoG a)
{
    cout<<" HoG "<<endl;
    cout<<a.size()<<endl;
    for(auto h:a) cout<<h<<' ';
    cout<<endl;
}
int randFont() {
    int fonts[] = {
            //            cv::FONT_HERSHEY_SIMPLEX,
            //            cv::FONT_HERSHEY_PLAIN,
            //            cv::FONT_HERSHEY_DUPLEX,
            //            cv::FONT_HERSHEY_COMPLEX,
            //            cv::FONT_HERSHEY_TRIPLEX,
            cv::FONT_HERSHEY_COMPLEX_SMALL,
            //            cv::FONT_HERSHEY_SCRIPT_SIMPLEX,
            //            cv::FONT_HERSHEY_SCRIPT_COMPLEX,
    };
    // Выбираем случайный шрифт из тех что есть в OpenCV
    int nfonts = (sizeof(fonts) / sizeof(int));
    int font = rand() % nfonts;

    // С вероятностью 20% делаем шрифт наклонным (italic)
    //    bool is_italic = ((rand() % 5) == 0);
    //    if  (is_italic) {
    //        font = font | cv::FONT_ITALIC;
    //    }

    return font;
}

double randFontScale() {
    double min_scale = 2.5;
    double max_scale = 3.0;
    double scale = min_scale + (max_scale - min_scale) * ((rand() % 100) / 100.0);
    return scale;
}

int randThickness() {
    int min_thickness = 2;
    int max_thickness = 3;
    int thickness = min_thickness + rand() % (max_thickness - min_thickness + 1);
    return thickness;
}

cv::Scalar randColor2() {
    return cv::Scalar(rand() % 128, rand() % 128, rand() % 128); // можно было бы брать по модулю 255, но так цвета будут темнее и контрастнее
}

cv::Mat generateImage(std::string text, int width=128, int height=128) {
    cv::Scalar white(255, 255, 255);
    cv::Scalar backgroundColor = white;
    // Создаем картинку на которую мы нанесем символ (пока что это просто белый фон)
    cv::Mat img(height, width, CV_8UC3, backgroundColor);

    // Выберем случайные параметры отрисовки текста - шрифт, размер, толщину, цвет
    int font = randFont();
    double fontScale = randFontScale();
    int thickness = randThickness();
    cv::Scalar color = randColor2();

    // Узнаем размер текста в пикселях (если его нарисовать с указанными параметрами)
    int baseline = 0;
    cv::Size textPixelSize = cv::getTextSize(text, font, fontScale, thickness, &baseline);

    // Рисуем этот текст идеально в середине картинки
    // (для этого и нужно было узнать размер текста в пикселях - чтобы сделать отступ от середины картинки)
    // (ведь при рисовании мы указываем координаты левого нижнего угла текста)
    int xLeft = (width / 2) - (textPixelSize.width / 2);
    int yBottom = (height / 2) + (textPixelSize.height / 2);
    cv::Point coordsOfLeftBorromCorner(xLeft, yBottom);
    cv::putText(img, text, coordsOfLeftBorromCorner, font, fontScale, color, thickness);

    return img;
}

HoG normalize(HoG a)
{
    double sum=0.0;for(auto h:a) sum+=h;
    for(auto &h:a) h/=sum;
    return a;
}
double dist(hogt a,hogt b)
{
    double ans=0;
    for(int i=0;i<a.size();++i) ans+=abs(a[i]-b[i]);
    return ans;
}
HoG buildHoG(cv::Mat grad_x, cv::Mat grad_y) {
    rassert(grad_x.type() == CV_32FC1, 2378274892374008);
    rassert(grad_y.type() == CV_32FC1, 2378274892374008);

    rassert(grad_x.rows == grad_y.rows, 3748247980010);
    rassert(grad_x.cols == grad_y.cols, 3748247980011);
    int height = grad_x.rows;
    int width = grad_x.cols;
    //std::cout<<height<<' '<<width<<std::endl;
    //cout<<" tgrfdewqs "<<endl;
    HoG hog;

    //
    // 1) увеличьте размер вектора hog до NBINS (ведь внутри это просто обычный вектор вещественных чисел
    hog.resize(NBINS);
    // 2) заполните его нулями
    fill(hog.begin(),hog.end(),0);
    // 3) пробегите по всем пикселям входной картинки и посмотрите на каждый градиент
    // (определенный двумя числами: dx проекцией на ось x в grad_x, dy проекцией на ось y в grad_y)
    // 4) определите его силу (корень из суммы квадратов), определите его угол направления:
    // рекомендую воспользоваться atan2(dy, dx) - он возвращает радианы - https://en.cppreference.com/w/cpp/numeric/math/atan2
    // прочитайте по ссылке на документацию (в прошлой строке) - какой диапазон значений у угла-результата atan2 может быть?
    // 5) внесите его силу как голос за соответствующую его углу корзину
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            float strength = sqrt(dx * dx + dy * dy);

            if (strength < 10) // пропускайте слабые градиенты, это нужно чтобы игнорировать артефакты сжатия в jpeg (например в line01.jpg пиксели не идеально белые/черные, есть небольшие отклонения)
                continue;

            //  рассчитайте в какую корзину нужно внести голос
            int bin = -1;
            double angle=atan2(dy,dx);
            angle+=pi;
            //cout<<angle<<" angle "<<endl;
            double cang=(2*pi/NBINS);
            bin=(angle/cang);
            if(bin==NBINS) --bin;
            rassert(bin >= 0, 3842934728039);
            rassert(bin < NBINS, 34729357289040);
            hog[bin] += strength;
        }
    }

    rassert(hog.size() == NBINS, 23478937290010);
    return hog;
}

// Эта функция просто преобразует картинку в черно-белую, строит градиенты и вызывает buildHoG объявленный выше - TODO ЕЕ ВАМ И НУЖНО ДОДЕЛАТЬ
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

    // TODO реализуйте эту функцию:
    HoG hog = buildHoG(grad_x, grad_y);
    return hog;
}

HoG buildHoG2(cv::Mat originalImg) {
    cv::Mat img = originalImg.clone();

    //rassert(img.type() == CV_8UC3, 347283678950077);

    //cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого

    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);

    // TODO реализуйте эту функцию:
    HoG hog = buildHoG(grad_x, grad_y);
    return hog;
}

// TODO реализуйте функцию которая позволит выводить гистограмму в консоль через std::cout << myHOG << std::endl;
// Пример корректного вывода (выводите не само значение накопленных голосов за каждое направление, а процент от общей суммы голосов):
// HoG[22.5=0%, 67.5=78%, 112.5=21%, 157.5=0%, 202.5=0%, 247.5=0%, 292.5=0%, 337.5=0%]
std::ostream &operator<<(std::ostream &os, const HoG &hog) {
    rassert(hog.size() == NBINS, 234728497230016);

    // TODO
    os << "HoG[";
    for (int bin = 0; bin < NBINS; ++bin) {
        //        os << angleInDegrees << "=" << percentage << "%, ";
    }
    os << "]";
    return os;
}

double pow2(double x) {
    return x * x;
}
/*std::vector<cv::Mat> splitSymbols(cv::Mat img)
{
    std::vector<cv::Mat> symbols;
    // TODO 101: чтобы извлечь кусок картинки (для каждого прямоугольника cv::Rect вокруг символа) - загуглите "opencv how to extract subimage"
    return symbols;
}*/
vector <hogt> v[26];
void generateAllLetters() {
    srand(239017); // фиксируем зерно генератора случайных чисел (чтобы картинки от раза к разу генерировались с одинаковыми шрифтами, размерами и т.п.)

    for (char letter = 'a'; letter <= 'z'; ++letter) {

        // Создаем папку для текущей буквы:
        std::string letterDir = LETTER_DIR_PATH + "/" + letter;
        std::filesystem::create_directory(letterDir);
        for (int sample = 1; sample <= NSAMPLES_PER_LETTER; ++sample) {
            std::string text = std::string("") + letter;
            cv::Mat img = generateImage(text);

            cv::blur(img, img, cv::Size(3, 3));

            std::string letterSamplePath = letterDir + "/" + std::to_string(sample) + ".png";
            cv::imwrite(letterSamplePath, img);
        }
    }
}
void go1() {
    // TODO Проведите эксперимент 1:
    // Пробежав в цикле по каждой букве - посчитайте насколько сильно она отличается между своими пятью примерами? (NSAMPLES_PER_LETTER)
    // Для каждой буквы выведите:
    // 1) Среднее попарное расстояние (среднюю похожесть) между всеми примерами этой буквы
    // 2) Максимальное попарное расстояние между примерами этой буквы
    //
    // А так же среди всех максимальных расстояний найдите максимальное и выведите его в конце

    std::cout << "________go1________" << std::endl;
    for (char letter = 'a'; letter <= 'z'; ++letter) {
        std::string letterDir = LETTER_DIR_PATH + "/" + letter;
        //cout<<letter<<" letter "<<endl;
        for (int sampleA = 1; sampleA <= NSAMPLES_PER_LETTER; ++sampleA) {
            cv::Mat a = cv::imread(letterDir + "/" + std::to_string(sampleA) + ".png");
            //cv::Mat b = cv::imread(letterDir + "/" + std::to_string(sampleB) + ".png");
            //cout<<" ytgrf "<<endl;
            HoG hogA = buildHoG(a);
            //cout<<letter<<endl;
            hogt ans=normalize(hogA);
            //cou(ans);
            v[letter-'a'].push_back(ans);
        }
    }
}
void prep()
{
    generateAllLetters();
    go1();
    //cout<<" generated all"<<endl;
}
string ch(cv::Mat img)
{
    string ans;
    hogt u=normalize(buildHoG2(img));
    //cou(u);
    pair <int,int> el={-1,-1};
    for(int i=0;i<26;++i) for(int j=0;j<NSAMPLES_PER_LETTER;++j)
    {
        if(el.first==(-1) || dist(u,v[el.first][el.second])>dist(u,v[i][j])) el={i,j};
    }
    double d=dist(u,v[el.first][el.second]);
    //cout<<d<<endl;
    if(d>=0.2) return ans;
    ans.push_back((char) ('a'+el.first));
    return ans;
}