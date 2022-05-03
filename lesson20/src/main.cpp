#include <omp.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <thread>
#include <iostream>
#include <filesystem>

#include <libutils/timer.h>
#include <libutils/rasserts.h>
#include <libutils/fast_random.h>

#include "helpers.h" // ЗДЕСЬ (и в helpers.cpp) ДЛЯ ВАС РЕАЛИЗОВАНЫ ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ И СТРУКТУРЫ, ПОСТРОЙТЕ АЛГОРИТМ НА ИХ БАЗЕ
#include "disjoint_set.h"
#include "constants.h"


void run(std::string caseName) {
    std::cout << "__________________Case " << caseName << "__________________" << std::endl;

    cv::Mat original = cv::imread("lesson20/data/" + caseName + ".jpg");
    rassert(!original.empty(), 324789374290018);

    std::cout << "Image resolution: " << original.cols << "x" << original.rows << std::endl;

    // создаем папку в которую будем сохранять результаты - lesson20/resultsData/ИМЯ_НАБОРА/
    std::string resultsDir = "lesson20/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    resultsDir += caseName + "/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    // сохраняем в папку с результатами оригинальную картинку и маску
    cv::imwrite(resultsDir + "00original.png", original);

    /* TODO скопируйте себе в блокнот список этих этапов, по мере изучения кода помечайте плюсиком (+) те этапы что хорошо поняли, минусиком (-) те что не поняли вовсе, +- те что где-то средне */
    /* TODO на момент сдачи задания у вас ДОЛЖЕН БЫТЬ ОКТРЫТ БЛОКНОТ с этими пометками (можете прямо закоммитить в репозиторий этот текстовый файл положив рядом с этим исходником) */
    // Итак хочется реализовать обнаружение черного прямоугольника:
    // 1) Сглаживаем картинку
    // 2) Свертка оператором Собеля (чтобы построить градиент в каждом пикселе)
    // 3) Строим граф на базе всех всех пикселей
    // 4) Сортируем ребра - разница в направлении градиентов смежных пикселей
    // 5) Проходя по ребрам по возрастанию - объединяем смежные компоненты если у них похожи диапазоны направлений и сил градиентов
    // 6) Каждая компонента связности - отрезок
    // 7) Перебором пытаемся объединить эти отрезки в четырехугольник

    cv::Mat img = original.clone();
    // 1) Сглаживаем картинку
    cv::blur(img, img, cv::Size(3, 3));
    cv::imwrite(resultsDir + "01blur.png", img);

    // 2) Свертка оператором Собеля (чтобы построить градиент в каждом пикселе)
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // преобразуем в оттенки серого
    cv::Mat grad_x, grad_y; // в этих двух картинках мы получим производную (градиент=gradient) по оси x и y
    // для этого достаточно дважды применить оператор Собеля (реализованный в OpenCV)
    cv::Sobel(img, grad_x, CV_32FC1, 1, 0);
    cv::Sobel(img, grad_y, CV_32FC1, 0, 1);
    rassert(!grad_x.empty(), 234892748239070017);
    rassert(!grad_y.empty(), 234892748239070018);
    cv::imwrite(resultsDir + "02sobelX.png", grad_x); // обратите внимание что не все границы как будто нашлись,
    cv::imwrite(resultsDir + "03sobelY.png", grad_y); // но на самом деле OpenCV просто игнорирует отрицательные градиенты, рисуя их так же как и ноль - черным

    cv::imwrite(resultsDir + "04sobelXnormalized.png", normalizedImage(grad_x, 0.0f, 255.0f)); // но в helpers.h/helpers.cpp подготовлена функция которая зажмет
    cv::imwrite(resultsDir + "05sobelYnormalized.png", normalizedImage(grad_y, 0.0f, 255.0f)); // картинку в рамки от 0 до 255

    // находим для каждого пикселя направление и силу градиента
    cv::Mat grad_direction = cv::Mat(img.rows, img.cols, CV_32FC1);
    cv::Mat grad_magnitude = cv::Mat(img.rows, img.cols, CV_32FC1);
    #pragma omp parallel for
    for (int j = 0; j < img.rows; ++j) {
        for (int i = 0; i < img.cols; ++i) {
            float dx = grad_x.at<float>(j, i);
            float dy = grad_y.at<float>(j, i);
            grad_magnitude.at<float>(j, i) = sqrtf(dx * dx + dy * dy);
            grad_direction.at<float>(j, i) = atan2(dy, dx); // возвращает в диапазоне [-pi, +pi]
        }
    }
    cv::imwrite(resultsDir + "06sobelDirection.png", normalizedImage(grad_direction, 0.0f, 255.0f));
    cv::imwrite(resultsDir + "07sobelMagnitude.png", normalizedImage(grad_magnitude, 0.0f, 255.0f));

    // 3) Строим граф на базе всех всех пикселей (для простоты - будем игнорировать крайние пиксели)
    const int nvertices = img.cols * img.rows;
    // 4) Сортируем ребра - разница в направлении градиентов смежных пикселей:
    std::vector<std::pair<float, std::pair<int, int> >> edges; // сначала надо просто создать все ребра (каждое ребро - это разница в направлении смежных пикселей И пара из индексов вершин)

    for (int j = 1; j + 1 < img.rows; ++j) {     // обходим все пиксели
        for (int i = 1; i + 1 < img.cols; ++i) { // для простоты - будем игнорировать крайние пиксели

            int vertexA = encodeVertex(j, i, img); // находим номер вершины для текущего пикселя

            for (int di = -1; di <= 1; ++di) {     // обходим все соседние пиксели
                for (int dj = -1; dj <= 1; ++dj) { // благодаря тому что мы игнорировали крайние пиксели - эти соседи всегда есть
                    int vertexB = encodeVertex(j + dj, i + di, img); // кодируем номер соседнего пикселя

                    if (vertexA <= vertexB) continue; // каждое ребро мы встретим дважды, поэтому пропускаем все ребра идущие в обратном направлении по номерам вершин

                    float directionA = grad_direction.at<float>(j, i);
                    float directionB = grad_direction.at<float>(j+dj, i+di);
                    float directionDiff = directionMinDiff(directionA, directionB); // разница в направлении смежных пикселей

                    // на базе примера 00 хочется добавить к приоритезации ребер следующее - хотим чтобы чаще объединения шли перпендикулярно направлению градиента
                    // т.е. мы хотим в первую очередб объединять пиксели ЛЕЖАЩИЕ НА ОДНОЙ ПРЯМОЙ (т.е. градиент перпендикулярен к прямой)
                    float neighborDirection = atan2(dj, di);
                    float sinA = sin(directionMinDiff(directionA, neighborDirection));
                    float sinB = sin(directionMinDiff(directionB, neighborDirection));
                    float howPerpendicularIsDirectionToNeighborDirection = std::max(sinA, sinB); // равно 1.0 если сосед от нас расположен в направлении перпендикулярном градиенту
                                                                                                 // равно 0.0 если сосед распаложен в перпендикулярном к градиенту направлении
                    rassert(howPerpendicularIsDirectionToNeighborDirection >= -1e-6 && howPerpendicularIsDirectionToNeighborDirection <= 1.0f+1e-6, 384724200113);

                    float magnitudeA = grad_magnitude.at<float>(j, i);
                    float magnitudeB = grad_magnitude.at<float>(j+dj, i+di);
                    float magnitudeDiff = std::abs(magnitudeB - magnitudeA);

                    float edgeDistance = directionDiff
                            + PRIORITY_WEIGHT_PERPENDICULARITY * howPerpendicularIsDirectionToNeighborDirection
                            + PRIORITY_WEIGHT_MAGNITUDE * magnitudeDiff;
                    std::pair<int, int> verticesPair(vertexA, vertexB);
                    edges.push_back(std::pair<float, std::pair<int, int>>(edgeDistance, verticesPair)); // каждое ребро - это разница в направлении смежных пикселей И пара из индексов вершин
                }
            }
        }
    }
    timer t;
    std::cout << "Sorting " << edges.size() << " edges..." << std::endl;
    std::sort(edges.begin(), edges.end()); // сортировка будет по возрастанию первого значения в паре, т.е. по возрастанию разницы, давайте это проверим:
    for (int i = 1; i < edges.size(); ++i) {
        rassert(edges[i - 1].first <= edges[i].first, 394729834728900112);
    }
    std::cout << "done in " << t.elapsed() << " s" << std::endl;

    // 5) Проходя по ребрам по возрастанию - объединяем смежные компоненты если у них похожи диапазоны направлений и сил градиентов
    // но сначала надо создать по компоненте связности на ребро
    DisjointSet disjoint_set(nvertices);
    std::vector<GradientsCluster> clusters; // кроме того в каждой компоненте связности хранится: минимальный и максимальный угол градиента, минимальная и максимальная сила градиента
    const float DISABLED_PIXELS_MAGNITUDE = 1e10;
    for (int j = 0; j < img.rows; ++j) {
        for (int i = 0; i < img.cols; ++i) {
            int clusterId = encodeVertex(j, i, img);
            rassert(clusterId == clusters.size(), 3472937412900126);
            float direction = grad_direction.at<float>(j, i);
            float magnitude = grad_magnitude.at<float>(j, i);
            if (magnitude < 10) {
                magnitude = DISABLED_PIXELS_MAGNITUDE; // таким образом все пиксели со слабым градиентом будут объединяться только с такими же неважными пикселями
            }
            clusters.push_back(GradientsCluster(direction, magnitude));
        }
    }
    rassert(clusters.size() == disjoint_set.size(), 34729347200130);
    // а вот теперь идем по редрам чтобы объединять компоненты связности
    t.restart();
    std::cout << "Merging gradient clusters..." << std::endl;
    for (int ei = 0; ei < edges.size(); ++ei) {
        std::pair<int, int> verticesPair = edges[ei].second;
        int vertexA = verticesPair.first;
        int vertexB = verticesPair.second;
        rassert(vertexA != vertexB, 3472983472136);

        int clusterIndexA = disjoint_set.get_set(vertexA);
        int clusterIndexB = disjoint_set.get_set(vertexB);
        if (clusterIndexA == clusterIndexB) // эти два пикселя уже в одном и том же кластере
            continue;
        GradientsCluster clusterA = clusters[clusterIndexA];
        GradientsCluster clusterB = clusters[clusterIndexB];
        /* TODO реализуйте здесь объединение двух кластеров - посмотрите, может у класса GradientsCluster есть подходящий для этого метод? */

        /* TODO реализуйте здесь формулы проверяющие можно ли объединить эти два кластера, т.е. можно ли использовать этот объединенный кластер */
        bool directionOk = true;
        bool magnitudeOk = true;
        if (directionOk && magnitudeOk) {
            int unionIndex = disjoint_set.union_sets(clusterIndexA, clusterIndexB);
            rassert(unionIndex == clusterIndexA || unionIndex == clusterIndexB, 3847239700149);
            /* clusters[unionIndex] = ...; */
        }
    }
    std::cout << "done in " << t.elapsed() << " s" << std::endl;

    int nverticesSum = 0;
    float directionRangeSum = 0.0f;
    float magnitudeRangeSum = 0.0f;
    int nclusters = 0;
    for (int vertex = 0; vertex < nvertices; ++vertex) { // мы хотим пробежаться по каждому кластеру чтобы добавить его диапазон направлений в общую сумму
        bool vertexIsTheRootOfTheSet = (disjoint_set.get_set(vertex) == vertex);  // проще всего это сделать пропуская все вершины
        if (!vertexIsTheRootOfTheSet)                                             // кроме тех что являются главными в своей компоненте
            continue;

        int clusterIndex = disjoint_set.get_set(vertex);
        rassert(clusterIndex == vertex, 74238974728169); // см. if выше, он это гарантирует
        if (clusters[clusterIndex].fromM != DISABLED_PIXELS_MAGNITUDE) {
            nverticesSum += disjoint_set.get_set_size(clusterIndex);
            directionRangeSum += clusters[clusterIndex].rangeD();
            magnitudeRangeSum += clusters[clusterIndex].rangeM();
            ++nclusters;
        }
    }
    rassert(nclusters <= disjoint_set.count_differents(), 3748293742800172);
    std::cout << nclusters << " clusters after merging:" << std::endl;
    std::cout << "avg pixels per cluster: " << (nverticesSum * 1.0 / nclusters) << std::endl;
    std::cout << "avg direction range per cluster: " << (directionRangeSum * 1.0 / nclusters) << " radians" << std::endl;
    std::cout << "avg magnitude range per cluster: " << (magnitudeRangeSum * 1.0 / nclusters) << " pixels" << std::endl;

    // 6) Каждая компонента связности - отрезок
    // для начала визуализируем их ввиде картинки
    std::unordered_map<int, cv::Vec3b> clusters_colors; // назначив каждой компоненте связности случайный цвет
    std::unordered_map<int, std::vector<cv::Point2i>> clusters_pixels; // кроме того давайте для каждого кластера сохраним перечень пикселей которые в него входят
    FastRandom randomColorsGenerator;
    cv::Mat clustersVisualization = cv::Mat(img.rows, img.cols, CV_8UC3);
    for (int vertex = 0; vertex < nvertices; ++vertex) {
        cv::Point2i pixel = decodeVertex(vertex, img);
        int i = pixel.x; // определили в каком пикселе мы находимся
        int j = pixel.y;

        int clusterIndex = disjoint_set.get_set(vertex); // определили в каком кластере состоит этот пиксель

        if (clusters_colors.count(clusterIndex) == 0) { // если цвет для этого кластера еще не назначен
            if (clusters[clusterIndex].fromM == DISABLED_PIXELS_MAGNITUDE && clusters[clusterIndex].toM == DISABLED_PIXELS_MAGNITUDE) {
                clusters_colors[clusterIndex] = cv::Vec3b(0, 0, 0); // если этот кластер состоит только из выключенных пикселей - пусть будет черным
            } else {
                /* TODO визуализируйте все компоненты связности, если этот clusterIndex мы видим впервые - назначьте ему случайный цвет */
            }
            rassert(clusters_pixels.count(clusterIndex) == 0, 23847293473200227); // проверяем что раз цвет кластеру неназначен, то и пикселей у этого кластера пока не добавлено
            clusters_pixels[clusterIndex] = std::vector<cv::Point2i>();
        }
        /* TODO используйте ранее назначенный этому кластеру цвет и покрасьте пиксель в этот цвет */
        /* TODO а так же добавьте текущий пиксель в перечень пикселей данного кластера */
    }
    cv::imwrite(resultsDir + "08clusters.png", clustersVisualization);

    t.restart();
    std::cout << "Estimating segments starts and finishes..." << std::endl;
    // теперь для каждого кластера надо выбрать начало и конец
    std::unordered_map<int, cv::Point2i> clusters_start;
    std::unordered_map<int, cv::Point2i> clusters_finish;
    // и давайте нарисуем эти отрезки (обведя кружком конец)
    cv::Mat segmentsVisualization = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    int goodSegments = 0;
    std::vector<int> goodClusters;
    for (int vertex = 0; vertex < nvertices; ++vertex) { // мы хотим пробежаться по каждому кластеру
        bool vertexIsTheRootOfTheSet = (disjoint_set.get_set(vertex) == vertex);
        if (!vertexIsTheRootOfTheSet)
            continue;

        int clusterIndex = disjoint_set.get_set(vertex);
        rassert(clusterIndex == vertex, 74238974728169); // см. if выше, он это гарантирует
        if (clusters[clusterIndex].fromM == DISABLED_PIXELS_MAGNITUDE)
            continue;

        std::vector<cv::Point2i> pixels = clusters_pixels[clusterIndex];
        Line line = fitLineFromPixels(pixels, grad_magnitude, grad_direction);
        cv::Point2i start, finish;
        projectPixelsAndChooseStartFinish(pixels, line, start, finish);
        clusters_start[clusterIndex] = start;
        clusters_finish[clusterIndex] = finish;
        if (pixels.size() > CLUSTER_NPOINTS_THRESHOLD) {
            /* TODO нарисуйте отрезки для каждого кластера - от стартовой к конечной точке с цветом этого кластера */
            /* TODO нарисуйте кружочек вокруг конца этого кластера - с цветом этого кластера, чтобы было понятно его направление */

            goodClusters.push_back(clusterIndex);
            ++goodSegments;
        }
    }
    std::cout << goodSegments << " good segments found (with >" << CLUSTER_NPOINTS_THRESHOLD << " pixels)" << std::endl;
    cv::imwrite(resultsDir + "09segments.png", segmentsVisualization);
    std::cout << "done in " << t.elapsed() << " s" << std::endl;

    // 7) Перебором пытаемся объединить эти отрезки в четырехугольник
    t.restart();
    std::cout << "Connecting quads..." << std::endl;
    std::vector<std::vector<int>> quadsClusters;
    for (int cluster0: goodClusters) { // перебираем первую сторону четырехугольника
        rassert(clusters_start.count(cluster0) > 0, 3492783890002277);
        cv::Point2i p0 = clusters_start[cluster0]; // эта первая точка - первый угол
        cv::Point2i p01 = clusters_finish[cluster0]; // вокруг этой точки мы ищем продолжение следующим кластером

        for (int cluster1: goodClusters) {
            rassert(clusters_start.count(cluster1) > 0, 3492783890002279);
            cv::Point2i p1 = clusters_start[cluster1];
            // если начало следующего кластера слишком далеко от конца предыдущего кластера - он нам не подходит
            if (distance2(p01, p1) > QUADS_CORNERS_DISTANCE_THRESHOLD2 || cluster1 == cluster0)
                continue;
            cv::Point2i p12 = clusters_finish[cluster1]; // теперь мы ищем продолжение вокруг этой точки

            for (int cluster2: goodClusters) { // перебираем третью сторону четырехугольника
                /* TODO сделайте проверку подошел ли нам рассматриваемый третий кластер */

                for (int cluster3: goodClusters) {
                    /* TODO сделайте проверку подошел ли нам рассматриваемый четвертый кластер */

                    /* TODO не забудьте добавить финальную проверку - оказался ли замкнут наш четырехугольник? */

                    {
                        // если все хорошо - добавляем четверку кластеров в печеречень четырехугольников
                        std::vector<int> clusters_ids = {cluster0, cluster1, cluster2, cluster3};
                        quadsClusters.push_back(clusters_ids);
                    }
                }
            }
        }
    }
    rassert(quadsClusters.size() % 4 == 0, 3472394328323); /* TODO почему это всегда верно? какие четырехугольники надо удалить а какие оставить? как это сделать? */
    std::cout << quadsClusters.size() << " quads found" << std::endl;
    cv::Mat quadsVisualization = cv::Mat(img.rows, img.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    for (int i = 0; i < quadsClusters.size(); ++i) {
        /* TODO добавьте визуализацию, отрисуйте каждый четырехугольник - прочертив 4 отрезка - по его сторонам, и 4 отрезка - между концами и началами последовательных сторон */
        /* TODO цветом четырехугольника пусть берется из цвета первого кластера который в этом четырехугольнике участвует */
    }
    cv::imwrite(resultsDir + "10quads.png", quadsVisualization);
    /* TODO посмотрите на результаты, какие есть идеи как уменьшить число ложных срабатываний? попробуйте это сделать */
    /* TODO посмотрите на результаты, как убрать срабатывания на белых квадратах? (ведь нас только черные интересуют, т.е. те что обходятся против часовой стрелки) */
    std::cout << "done in " << t.elapsed() << " s" << std::endl;
}

int omp_thread_count() {
    // эта функция определяет число OpenMP потоков
    int nthreads = 0;
    #pragma omp parallel reduction(+: nthreads)
    {
        nthreads += 1;
    }
    return nthreads;
}

int main() {
    try {
        int nthreads = omp_thread_count();
        std::cout << "CPU on this computer has " << nthreads << " virtual threads" << std::endl;

        run("00");
        run("01");
        run("02");
        run("03");
        run("04");
        run("05");

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
