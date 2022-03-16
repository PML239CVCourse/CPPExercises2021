#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <set>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <memory>

#include <libutils/rasserts.h>


bool isPixelEmpty(cv::Vec3b color) {
    return false; // TODO скопируйте эту функцию из прошлого задания про построение картинки-визуализации разницы картинок
}

// Эта функция построит лабиринт - в каждом пикселе будет число которое говорит насколько длинное или короткое ребро выходит из пикселя
cv::Mat buildTheMaze(cv::Mat pano0, cv::Mat pano1) {
    rassert(!pano0.empty(), 324783479230019);
    rassert(!pano1.empty(), 324783479230020);
    rassert(pano0.type() == CV_8UC3, 3447928472389021);
    rassert(pano1.type() == CV_8UC3, 3447928472389022);
    rassert(pano0.cols == pano1.cols, 3295782390572300071);
    rassert(pano0.rows == pano1.rows, 3295782390572300072);

    const int MIN_PENALTY = 1;
    const int BIG_PENALTY = 100000;
    cv::Mat maze(pano0.rows, pano0.cols, CV_32SC1, cv::Scalar(0)); // создали лабиринт, размером с панораму, каждый пиксель - int

    for (int j = 0; j < pano0.rows; ++j) {
        for (int i = 0; i < pano0.cols; ++i) {
            cv::Vec3b color0 = pano0.at<cv::Vec3b>(j, i);
            cv::Vec3b color1 = pano1.at<cv::Vec3b>(j, i);

            int penalty = 0; // TODO найдите насколько плохо идти через этот пиксель:
            // BIG_PENALTY - если этот пиксель отсутствует в pano0 или в pano1
            // разница между цветами этого пикселя в pano0 и в pano1 (но не меньше MIN_PENALTY)

            maze.at<int>(j, i) = penalty;
        }
    }

    return maze;
}

struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

int encodeVertex(int row, int column, int nrows, int ncolumns) {
    // TODO скопируйте эту функцию из задания про Дейкстру на картинке лабиринта
    return 0;
}

cv::Point2i decodeVertex(int vertexId, int nrows, int ncolumns) {
    // TODO скопируйте эту функцию из задания про Дейкстру на картинке лабиринта
    int column = 0;
    int row = 0;
    return cv::Point2i(column, row);
}

// Скопируйте в эту функцию Дейкстру из позапрошлого задания - mainMaze.cpp
std::vector<cv::Point2i> findBestSeam(cv::Mat maze, cv::Point2i startPoint, cv::Point2i finishPoint) {
    rassert(!maze.empty(), 324783479230019);
    rassert(maze.type() == CV_32SC1, 3447928472389020);
    std::cout << "Maze resolution: " << maze.cols << "x" << maze.rows << std::endl;

    int nvertices = maze.cols * maze.rows;

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            int w = maze.at<int>(j, i);

            // TODO добавьте в edges_by_vertex ребро вправо и вверх с длинной w
        }
    }

    int start = encodeVertex(startPoint.y, startPoint.x, maze.rows, maze.cols);
    int finish = encodeVertex(finishPoint.y, finishPoint.x, maze.rows, maze.cols);
    rassert(start >= 0 && start < nvertices, 348923840234200127);
    rassert(finish >= 0 && finish < nvertices, 348923840234200128);

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> distances(nvertices, INF);
    // TODO СКОПИРУЙТЕ СЮДА ДЕЙКСТРУ ИЗ ПРЕДЫДУЩЕГО ИСХОДНИКА - mainMaze.cpp
    // ...

    // убеждаемся что мы добрались до финиша
//    rassert(processed[finish], 3478289374239000163);

    // TODO извлекаем вершины через которые прошел кратчайший путь:
    // std::vector<int> path;
    //int cur = finish;
    //while (cur != -1) {
    //    path.push_back(cur);
    //    cur = parent[cur];
    //}
    std::vector<cv::Point2i> pathPoints;
    //for (int i = path.size() - 1; i >= 0; --i) {
    //    cv::Point2i p = decodeVertex(path[i], maze.rows, maze.cols);
    //    pathPoints.push_back(p);
    //}
    return pathPoints;
}


void run(std::string caseName) {
    cv::Mat img0 = cv::imread("lesson16/data/" + caseName + "/0.png");
    cv::Mat img1 = cv::imread("lesson16/data/" + caseName + "/1.png");
    rassert(!img0.empty(), 324789374290018);
    rassert(!img1.empty(), 378957298420019);

    int downscale = 2; // уменьшим картинку в два раза столько раз сколько указано в этой переменной (итоговое уменьшение в 2^downscale раз)
    for (int i = 0; i < downscale; ++i) {
        cv::pyrDown(img0, img0); // уменьшаем картинку в два раза (по каждой из осей)
        cv::pyrDown(img1, img1); // уменьшаем картинку в два раза (по каждой из осей)
    }

    cv::Ptr<cv::FeatureDetector> detector = cv::SIFT::create();
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);

    // Детектируем и описываем дескрипторы ключевых точек
    std::vector<cv::KeyPoint> keypoints0, keypoints1;
    cv::Mat descriptors0, descriptors1;
    detector->detectAndCompute(img0, cv::noArray(), keypoints0, descriptors0);
    detector->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
    std::cout << "Keypoints initially: " << keypoints0.size() << ", " << keypoints1.size() << "..." << std::endl;

    // Сопоставляем ключевые точки (находя для каждой точки из первой картинки - две ближайшие, т.е. две самые похожие, с правой картинки)
    std::vector<std::vector<cv::DMatch>> matches01;
    matcher->knnMatch(descriptors0, descriptors1, matches01, 2);
    rassert(keypoints0.size() == matches01.size(), 349723894200068);

    // Фильтруем сопоставления от шума - используя K-ratio тест
    std::vector<cv::Point2f> points0;
    std::vector<cv::Point2f> points1;
    for (int i = 0; i < keypoints0.size(); ++i) {
        int fromKeyPoint0 = matches01[i][0].queryIdx;
        int toKeyPoint1Best = matches01[i][0].trainIdx;
        float distanceBest = matches01[i][0].distance;
        rassert(fromKeyPoint0 == i, 348723974920074);
        rassert(toKeyPoint1Best < keypoints1.size(), 347832974820076);

        int toKeyPoint1SecondBest = matches01[i][1].trainIdx;
        float distanceSecondBest = matches01[i][1].distance;
        rassert(toKeyPoint1SecondBest < keypoints1.size(), 3482047920081);
        rassert(distanceBest <= distanceSecondBest, 34782374920082);

        // простой K-ratio тест, но могло иметь смысл добавить left-right check
        if (distanceBest < 0.7 * distanceSecondBest) {
            points0.push_back(keypoints0[i].pt);
            points1.push_back(keypoints1[toKeyPoint1Best].pt);
        }
    }
    rassert(points0.size() == points1.size(), 234723947289089);
    std::cout << "Matches after K-ratio test: " << points0.size() << std::endl;

    // Находим матрицу преобразования второй картинки в систему координат первой картинки
    cv::Mat H10 = cv::findHomography(points1, points0, cv::RANSAC, 3.0);
    rassert(H10.size() == cv::Size(3, 3), 3482937842900059); // см. документацию https://docs.opencv.org/4.5.1/d9/d0c/group__calib3d.html#ga4abc2ece9fab9398f2e560d53c8c9780
                                                                             // "Note that whenever an H matrix cannot be estimated, an empty one will be returned."

    // создаем папку в которую будем сохранять результаты - lesson17/resultsData/ИМЯ_НАБОРА/
    std::string resultsDir = "lesson17/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    resultsDir += caseName + "/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }

    cv::imwrite(resultsDir + "0img0.jpg", img0);
    cv::imwrite(resultsDir + "1img1.jpg", img1);

    // находим куда переходят углы второй картинки
    std::vector<cv::Point2f> corners1(4);
    corners1[0] = cv::Point(0, 0); // верхний левый
    corners1[1] = cv::Point(img1.cols, 0); // верхний правый
    corners1[2] = cv::Point(img1.cols, img1.rows); // нижний правый
    corners1[3] = cv::Point(0, img1.rows); // нижний левый
    std::vector<cv::Point2f> corners10(4);
    perspectiveTransform(corners1, corners10, H10);

    // находим какой ширины и высоты наша панорама (как минимум - разрешение первой картинки, но еще нужно учесть куда перешли углы второй картинки)
    int max_x = img0.cols;
    int max_y = img0.rows;
    for (int i = 0; i < 4; ++i) {
        max_x = std::max(max_x, (int) corners10[i].x);
        max_y = std::max(max_y, (int) corners10[i].y);
    }
    int pano_rows = max_y;
    int pano_cols = max_x;

    // преобразуем обе картинки в пространство координат нашей искомой панорамы
    cv::Mat pano0(pano_rows, pano_cols, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat pano1(pano_rows, pano_cols, CV_8UC3, cv::Scalar(0, 0, 0));
    rassert(img0.type() == CV_8UC3, 3423890003123093);
    rassert(img1.type() == CV_8UC3, 3423890003123094);
    // вторую картинку просто натягиваем в соответствии с ранее найденной матрицей Гомографии
    cv::warpPerspective(img1, pano1, H10, pano1.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    // первую картинку надо положить без каких-то смещений, т.е. используя единичную матрицу:
    cv::Mat identity_matrix = cv::Mat::eye(3, 3, CV_64FC1);
    cv::warpPerspective(img0, pano0, identity_matrix, pano1.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);

    cv::imwrite(resultsDir + "2pano0.jpg", pano0);
    cv::imwrite(resultsDir + "3pano1.jpg", pano1);

    // давайте сделаем наивную панораму - наложим вторую картинку на первую:
    cv::Mat panoBothNaive = pano0.clone();
    cv::warpPerspective(img1, panoBothNaive, H10, panoBothNaive.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    cv::imwrite(resultsDir + "4panoBothNaive.jpg", panoBothNaive);

    // TODO: построим лабиринт (чем больше значение в пикселе - тем "хуже" через него проходить)
    cv::Mat maze = buildTheMaze(pano0, pano1); // TODO реализуйте построение лабиринта на базе похожести наложенных друг на друга картинок

    // найдем оптимальный шов разделяющий обе картинки (кратчайший путь в лабиринте)
    cv::Point2i start(0, pano_rows - 1); // из нижнего левого угла
    cv::Point2i finish(pano_cols - 1, 0); // в верхний правый угол
    std::cout << "Searching for optimal seam..." << std::endl;
    std::vector<cv::Point2i> seam = findBestSeam(maze, start, finish); // TODO реализуйте в этой функции Дейкстру
    for (int i = 0; i < seam.size(); ++i) {
        // cv::Point2i pointOnSeam = seam[i];
        // TODO рисуем красный шов там где мы нашли наш лучший шов
        // panoBothNaive.at<cv::Vec3b>(???, ???) = cv::Vec3b(0, 0, 255);
    }
    std::cout << "Length of optimal seam: " << seam.size() << " pixels" << std::endl;
    cv::imwrite(resultsDir + "5panoOptimalSeam.jpg", panoBothNaive);

    // TODO теперь надо поиском в ширину (см. описание на сайте) разметить пиксели:
    const unsigned char PIXEL_NO_DATA = 0; // черные без информации (не покрыты картинкой)
    const unsigned char PIXEL_IS_ON_SEAM = 1; // те что лежат на шве (через них первая картинка не перешагивает, но в конечном счете давайте на шве рисовать вторую картинку)
    const unsigned char PIXEL_FROM_PANO0 = 100; // те что покрыты первой картинкой (сверху слева от шва)
    const unsigned char PIXEL_FROM_PANO1 = 200; // те что покрыты второй картинкой (справа снизу от шва)
    cv::Mat sourceId(pano_rows, pano_cols, CV_8UC1, cv::Scalar(PIXEL_NO_DATA));
    for (int i = 0; i < seam.size(); ++i) {
        // TODO заполните писели лежащие на шве, чтобы легко было понять что через них перешагивать нельзя:
        // sourceId.at<unsigned char>(pointOnSeam.y, pointOnSeam.x) = PIXEL_IS_ON_SEAM;
    }

    // TODO левый верхний угол - точно из первой картинки - отмечаем его и добавляем в текущую волну для обработки
    cv::Point2i leftUpCorner(0, 0);
    sourceId.at<unsigned char>(leftUpCorner.y, leftUpCorner.x) = PIXEL_FROM_PANO0;
    std::vector<cv::Point2i> curWave;
    curWave.push_back(leftUpCorner);

    while (curWave.size() > 0) {
        std::vector<cv::Point2i> nextWave;
        for (int i = 0; i < curWave.size(); ++i) {
            cv::Point2i p = curWave[i];

            // кодируем сдвиг координат всех четырех соседей:
            //            слева (dx=-1, dy=0), сверху (dx=0, dy=-1), справа (dx=1, dy=0), снизу (dx=0, dy=1)
            int dxs[4] = {-1,                   0,                   1,                   0};
            int dys[4] = {0,                   -1,                   0,                   1};

            for (int k = 0; k < 4; ++k) { // смотрим на четырех соседей
                int nx = p.x + dxs[k];
                int ny = p.y + dys[k];
                // TODO посмотрите на соседний пиксель (nx, ny) и либо отметьте его как покрытый первой картинкой и добавьте в следующую волну, либо проигнорируйте
                // см. описание на сайте
            }
        }
        curWave = nextWave;
    }
    for (int j = 0; j < pano_rows; ++j) {
        for (int i = 0; i < pano_cols; ++i) {
            // TODO отметьте все остальные пиксели как пиксели второй картинки
        }
    }
    cv::imwrite(resultsDir + "6sourceId.jpg", sourceId);

    cv::Mat newPano(pano_rows, pano_cols, CV_8UC3, cv::Scalar(0, 0, 0));
    // TODO постройте новую панораму в соответствии с sourceId картой (забирая цвета из pano0 и pano1)
    cv::imwrite(resultsDir + "7newPano.jpg", newPano);
}


int main() {
    try {
        run("1_hanging");
        run("2_hiking");
        run("3_aero");
//        run("4_your_data"); // TODO сфотографируйте что-нибудь сами при этом на второй картинке что-то изменив, см. иллюстрацию на сайте

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
