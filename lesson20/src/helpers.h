#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#define PI ((float) CV_PI)

// Эта функция находит минимальное и максимальное вещественное значение в картинке
// и создает копию этой картинке с нормализованными значениями (т.е. значения оказываются зажаты в диапазоне от 0 до 1)
cv::Mat normalizedImage(const cv::Mat &img, float from, float to);

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, const cv::Mat &img);

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, const cv::Mat &img);

// Эта функция находит кратчайшую разницу между направлениями (предполагая что они в диапазоне между [-pi, +pi])
// т.е. заметает угол либо от d0 до d1, либо в обратном направлении - в зависимости от того какой путь короче
float directionMinDiff(float d0, float d1);

// Эта функция находит знаковую разницу между направлениями (предполагая что они в диапазоне между [-pi, +pi])
// т.е. заметает угол от d0 до d1
float directionDiff(float d0, float d1);

// Эта структура будет хранить компоненту связности - кластер из похожих градиентов
struct GradientsCluster {
    float fromD;
    float toD;
    float fromM;
    float toM;
    int nPoints;

    GradientsCluster(float direction, float magnitude); // конструктор, изначально мы создаем кластер по одному пикселю, т.е. по одному градиенту

    float rangeD() const { return directionDiff(fromD, toD); }
    float rangeM() const { return toM - fromM; }

    GradientsCluster plus(const GradientsCluster &cluster) const; // эта функция добавит к нашему кластер другой кластер, и вернет кластер с более широким диапазоном
};

class Line {
public:
    double a, b, c;
    Line(double a, double b, double c) : a(a), b(b), c(c)
    {}
    double getXFromY(double x) const; // метод прямой позволяющий рассчитать точку на прямой (найти x по y)
    double getYFromX(double x) const; // метод прямой позволяющий рассчитать точку на прямой (найти y по x)
    double distanceFromPoint(double x, double y) const;
    double distance2FromPoint(double x, double y) const;
};

Line fitLineFromPixel(cv::Point2i pixel, float magnitude, float direction);
Line fitLineFromPixels(const std::vector<cv::Point2i> &pixels, const cv::Mat &grad_magnitude, const cv::Mat &grad_direction);
void projectPixelsAndChooseStartFinish(const std::vector<cv::Point2i> &pixels, Line line, cv::Point2i &start, cv::Point2i &finish);

int distance2(cv::Point2i a, cv::Point2i b);
