#include "helpers.h"

#include <libutils/rasserts.h>


// Эта функция находит минимальное и максимальное вещественное значение в картинке
// и создает копию этой картинке с нормализованными значениями (т.е. значения оказываются зажаты в диапазоне от 0 до 1)
cv::Mat normalizedImage(const cv::Mat &img, float from, float to)
{
    rassert(img.type() == CV_32FC1, 5487234985723005);
    rassert(from <= to, 4287389472980011);
    cv::Mat res = img.clone();

    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    cv::minMaxLoc(img, &minVal, &maxVal, &minLoc, &maxLoc);

    double rangeVal = (maxVal - minVal);
    if (rangeVal == 0.0) rangeVal = 1.0; // защищяемся от деления на ноль
    #pragma omp parallel for
    for (int j = 0; j < img.rows; ++j) {
        for (int i = 0; i < img.cols; ++i) {
            float x = (img.at<float>(j, i) - minVal) / rangeVal;
            x = from + x * (to - from);
            res.at<float>(j, i) = x;
        }
    }

    return res;
}

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, const cv::Mat &img) {
    int nrows = img.rows;
    int ncolumns = img.cols;

    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, 347823974239870018);
    int vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, const cv::Mat &img) {
    int nrows = img.rows;
    int ncolumns = img.cols;

    int row = vertexId / ncolumns;
    int column = vertexId % ncolumns;

    // сверим что функция симметрично сработала:
    rassert(encodeVertex(row, column, img) == vertexId, 34782974923035);

    rassert(row < nrows, 34723894720027);
    rassert(column < ncolumns, 3824598237592030);
    return cv::Point2i(column, row);
}

void rassertAngleInRange(float angle, float from=-PI, float to=PI) {
    rassert(angle >= from, 38472389420085);
    rassert(angle <= to, 38472389420086);
}

// Эта функция находит кратчайшую разницу между направлениями (предполагая что они в диапазоне между [-pi, +pi])
// т.е. заметает угол либо от d0 до d1, либо в обратном направлении - в зависимости от того какой путь короче
float directionMinDiff(float d0, float d1) {
    float min = -PI;
    float max = PI;
    rassertAngleInRange(d0, min, max);
    rassertAngleInRange(d1, min, max);

    if (d0 > d1) std::swap(d0, d1); // теперь мы упорядочили эти два направления
    rassert(d0 <= d1, 34927839047290059);

    float a = d1 - d0; // какой угол между направлениями напрямин - БЕЗ ПЕРЕСЕЧЕНИЯ угла -pi/+pi
    float b = (max - d1) + (d0 - min); // какой угол между направлениями С ПЕРЕСЕЧЕНИЕМ угла -pi/+pi
    rassert(a >= 0.0f, 38247390075);
    rassert(b >= 0.0f, 38247390076);

    float diff = std::min(a, b);
    rassert(diff >= 0.0f, 34958722890079);
    rassert(diff <= PI, 38742934720079); // кратчайший угол должен быть меньше pi
    return diff;
}

// Эта функция находит знаковую разницу между направлениями (предполагая что они в диапазоне между [-pi, +pi])
// т.е. заметает угол от d0 до d1
float directionDiff(float d0, float d1) {
    rassertAngleInRange(d0);
    rassertAngleInRange(d1);
    if (d1 < d0) {
        d1 += 2 * PI;
    }
    float diff = d1 - d0;
    rassert(diff <= 2 * PI, 2347283947230097);
    return diff;
}

GradientsCluster::GradientsCluster(float direction, float magnitude) {
    rassertAngleInRange(direction);

    fromD = direction;
    toD = direction;
    fromM = magnitude;
    toM = magnitude;
    nPoints = 1;
}

float clampAngleToRange(float angle, float from=-PI, float to=PI) {
    while (angle > to) {
        angle -= 2 * PI;
    }
    rassertAngleInRange(angle, from, to);
    return angle;
}

float diff(float a, float b) {
    return std::abs(a - b);
}

// Эта функция добавит к нашему кластер другой кластер, и вернет кластер с более широким диапазоном
GradientsCluster GradientsCluster::plus(const GradientsCluster &cluster) const {
    GradientsCluster sum = cluster;

    // сначала объединим диапазон сил градиентов, это просто
    sum.fromM = std::min(fromM, cluster.fromM); // выбираем минимум среди нашего минимума и минимума второго кластера
    sum.toM = std::max(toM, cluster.toM); // то же самое для максимума

    // теперь объединим диапазон направлений градиентов
    {
        rassert(fromD >= -PI && fromD <= PI, 234127839170094);
        rassert(toD >= -PI && toD <= PI, 234127839170095);
        rassert(cluster.fromD >= -PI && cluster.fromD <= PI, 234127839170096);
        rassert(cluster.toD >= -PI && cluster.toD <= PI, 234127839170097);

        float a = fromD;
        float b = toD;
        if (b < a) { // такое может быть если диапазон углов переходит через угол -pi/+pi
            b += 2 * PI; // теперь b-a = диапазону угла, дальше будет удобно что угол растет вверх без перепрыгивания с +pi на -pi (из-за зацикленности)
        }
        rassert(a <= b, 2389472938740095);

        float c = cluster.fromD;
        float d = cluster.toD;
        if (d < c) {
            d += 2 * PI;
        }
        rassert(c <= d, 2389472938740096);

        if (a > c) {
            std::swap(a, c);
            std::swap(b, d);
        }

        float rangeD1 = std::max(b, d) - a; // какой угол будет при объединении от угла (a-b) до угла (c-d)

        // переводим угло (a-b) на следующий виток 2*pi цикла:
        float a2 = a + 2 * PI;
        float b2 = b +2 * PI;
        float rangeD2 = std::max(b2, d) - c; // какой угол будет при объединении от угла (c-d) до угла (a-b)
        const float EPSILON = 1e-6;
        if (rangeD1 < rangeD2) {
            sum.fromD = clampAngleToRange(a);
            sum.toD = clampAngleToRange(std::max(b, d));
            float rangeD = sum.rangeD();
            rassert(diff(rangeD, rangeD1) < EPSILON || diff(rangeD, 2 * PI - rangeD1) < EPSILON, 3472839471200113);
        } else {
            sum.fromD = clampAngleToRange(c);
            sum.toD = clampAngleToRange(std::max(b2, d));
            float rangeD = sum.rangeD();
            rassert(diff(rangeD, rangeD2) < EPSILON || diff(rangeD, 2 * PI - rangeD2) < EPSILON, 3472839471200117);
        }
    }

    sum.nPoints = nPoints + cluster.nPoints;

    // всякие проверки на разумность результата:
    rassert(sum.rangeD() >= std::max(rangeD(), cluster.rangeD()), 3472384720121);
    rassert(sum.rangeM() >= std::max(rangeM(), cluster.rangeM()), 3472384720122);

    return sum;
}

double Line::getXFromY(double y) const { // метод прямой позволяющий рассчитать точку на прямой (найти x по y)
    rassert(std::abs(a) > 0.001, 2734832748932790023); // случай горизонтальной прямой не рассматривается для простоты
    double x = (b * y + c) / -a;
    return x;
}

double Line::getYFromX(double x) const { // метод прямой позволяющий рассчитать точку на прямой (найти y по x)
    rassert(std::abs(b) > 0.001, 2734832748932790061); // случай вертикальной прямой не рассматривается для простоты
    double y = (a * x + c) / -b;
    return y;
}

double Line::distanceFromPoint(double x, double y) const {
    rassert(a != 0.0 || b != 0.0, 384723947200187);
    double res = std::abs(a * x + b * y + c) / std::sqrt(a * a + b * b);
    return res;
}

double Line::distance2FromPoint(double x, double y) const {
    rassert(a != 0.0 || b != 0.0, 384723947200187);
    double abc = (a * x + b * y + c);
    double res = abc * abc / (a * a + b * b);
    return res;
}

Line fitLineFromPixel(cv::Point2i pixel, float direction) {
    float a = -cos(direction);
    float b = -sin(direction);
    float c = -(a * pixel.x + b * pixel.y);
    Line line(a, b, c);
    float pixelDistance = line.distanceFromPoint(pixel.x, pixel.y);
    rassert(pixelDistance < 1e-3, 34587293800197);
    return line;
}

Line fitLineFromPixels(const std::vector<cv::Point2i> &pixels, const cv::Mat &grad_magnitude, const cv::Mat &grad_direction){
    Line bestLine(-1.0, -1.0, -1.0);
    const float INF = std::numeric_limits<float>::max();
    float bestLineTotalDist = INF;
    #pragma omp parallel for
    for (int pi = 0; pi < pixels.size(); ++pi) {
        cv::Point2i px = pixels[pi];
        int i = px.x;
        int j = px.y;
        float direction = grad_direction.at<float>(j, i);

        Line line = fitLineFromPixel(px, direction); // текущий пиксель своим градиентом задал некоторую прямую (которая перпендикулярна этому градиенту)
        float lineTotalDist = 0.0f;

        for (int pj = 0; pj < pixels.size(); ++pj) {
            cv::Point2i p = pixels[pi];
            float distance = line.distanceFromPoint(p.x, p.y); // каждая точка смотрит насколько она далеко от предлагаемой прямой
            float distance2 = line.distance2FromPoint(p.x, p.y);

            float directionj = grad_direction.at<float>(p.y, p.x); // учитываем так же насколько различаются направления
            float magnitudej = grad_magnitude.at<float>(p.y, p.x); // и вносит свой вклад с учетом своей силы градиента

            float directionDiff = directionMinDiff(direction, directionj);
            lineTotalDist += distance + directionDiff;
        }

        if (lineTotalDist < bestLineTotalDist) {
            #pragma omp critical
            {
                if (lineTotalDist < bestLineTotalDist) {
                    bestLine = line;
                    bestLineTotalDist = lineTotalDist;
                }
            }
        }
    }
    rassert(bestLineTotalDist < INF, 347238947200229);
    return bestLine;
}

void projectPixelsAndChooseStartFinish(const std::vector<cv::Point2i> &pixels, Line line, cv::Point2i &start, cv::Point2i &finish) {
    cv::Point2f linePointA; // произвольная точка А на прямой
    cv::Point2f linePointB; // произвольная точка B на прямой
    if (std::abs(line.b) > std::abs(line.a)) { // если прямая больше горизонтальная чем вертикальная
        linePointA = cv::Point2f(0.0, line.getYFromX(0.0));
        linePointB = cv::Point2f(1.0, line.getYFromX(1.0));
    } else { // если прямая больше вертикальная чем горизонтальная
        linePointA = cv::Point2f(line.getXFromY(0.0), 0.0);
        linePointB = cv::Point2f(line.getXFromY(1.0), 1.0);
    }

    cv::Point2f lineNormal(line.a, line.b); // вектор нормали прямой (идет "вправо" от направления прямой)
    rassert(diff(line.a * line.a + line.b * line.b, 1.0) < 1e-6, 2837129000237);
    cv::Point2f AB = linePointB - linePointA; // вектор из А в B
    if (AB.cross(lineNormal) < 0.0) { // если векторное произведение положительное - то правило правого буравчика говорит нам что вектор из А в В идет по прямой
        std::swap(linePointA, linePointB);// но если векторное произведение меньше нуля - то вектор идет в противоположную прямой сторону
        AB = linePointB - linePointA;
    }
    // теперь мы знаем что вектор из А в В сонаправлен с прямой
    cv::Point2i closestToStart;
    float minT = std::numeric_limits<float>::max();
    cv::Point2i closestToFinish;
    float maxT = -std::numeric_limits<float>::max();
    for (int i = 0; i < pixels.size(); ++i) {
        cv::Point2f X = cv::Point2f(pixels[i]);
        // мы знаем что любая точка на прямой имеет вид Y = t*A + (1.0-t)*B
        // пусть мы хотим найти для точки X проекцию Y на прямую, тогда можно вывести - https://stackoverflow.com/a/64333474
        // формулу для t:
        cv::Point2f XB = linePointB - X;
        float t = XB.dot(AB) / AB.dot(AB);
        // и соответственно точка проекции Y = t*A + (1.0-t)*B
        cv::Point2f Y = cv::Point2f(linePointA) * t + (1.0 - t) * cv::Point2f(linePointB);
        if (t < minT) {
            minT = t;
            closestToStart = X;
            start = cv::Point2i(Y);
        }
        if (t > maxT) {
            maxT = t;
            closestToFinish = X;
            finish = cv::Point2i(Y);
        }
    }
    rassert(minT != std::numeric_limits<float>::max(), 3487238947200283);
    rassert(maxT != -std::numeric_limits<float>::max(), 3487238947200284);
}

int distance2(cv::Point2i a, cv::Point2i b) {
    cv::Point2i ab = b - a;
    int res = ab.dot(ab);
    return res;
}
