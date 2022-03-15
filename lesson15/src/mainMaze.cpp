#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>


struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, int nrows, int ncolumns) {
    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, column);
    int vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, int nrows, int ncolumns) {

    // TODO: придумайте как найти номер строки и столбика пикселю по номеру вершины (просто поймите предыдущую функцию и эта функция не будет казаться сложной)
    int row = vertexId/ncolumns;
    int column = vertexId-row*ncolumns;
    if (column >= ncolumns){}

    // сверим что функция симметрично сработала:
    rassert(encodeVertex(row, column, nrows, ncolumns) == vertexId, 34782974923035);

    rassert(row < nrows, 34723894720027);
    rassert(column < ncolumns, 3824598237592030);
    return cv::Point2i(column, row);
}

void run(int mazeNumber) {
    cv::Mat maze = cv::imread("lesson15/data/mazesImages/maze" + std::to_string(mazeNumber) + ".png");
    rassert(!maze.empty(), 324783479230019);
    rassert(maze.type() == CV_8UC3, 3447928472389020);
    std::cout << "Maze resolution: " << maze.cols << "x" << maze.rows << std::endl;

    int nvertices = maze.cols*maze.rows; // TODO

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            cv::Vec3b color = maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            int w1 = -1, w2 = 1, h1 = -1, h2 = 1, w = maze.rows-1, h = maze.cols-1;
            if(j == 0){
                w1 = 0;
            }
            if (j == w){
                w2 = 0;
            }
            if (i == 0){
                h1 = 0;
            }
            if (i == h){
                h2 = 0;
            }
            int bi = encodeVertex(j, i, maze.rows, maze.cols);
            for (int k = w1; k <= w2; ++k) {
                for (int l = h1; l <= h2; ++l) {
                    cv::Vec3b color1 = maze.at<cv::Vec3b>(j+k, i+l);
                    int delta = 1+(abs(color[0]-color1[0])+abs(color[2]-color1[2])+abs(color[2]-color1[2]))*nvertices;
                    int ai = encodeVertex(j+k, i+l, maze.rows, maze.cols);
                    edges_by_vertex[bi].push_back(Edge(bi, ai, delta));
                }
            }
            // TODO добавьте соотвтетсвующие этому пикселю ребра
        }
    }

    std::cout << "finish coding" << std::endl;

    int start, finish;
    if (mazeNumber >= 1 && mazeNumber <= 3) { // Первые три лабиринта очень похожи но кое чем отличаются...
        start = encodeVertex(300, 300, maze.rows, maze.cols);
        finish = encodeVertex(0, 305, maze.rows, maze.cols);
        std::cout << "start & finish done" << std::endl;
    } else if (mazeNumber == 4) {
        start = encodeVertex(154, 312, maze.rows, maze.cols);
        finish = encodeVertex(477, 312, maze.rows, maze.cols);
    } else if (mazeNumber == 5) { // Лабиринт в большом разрешении, добровольный (на случай если вы реализовали быструю Дейкстру с приоритетной очередью)
        start = encodeVertex(1200, 1200, maze.rows, maze.cols);
        finish = encodeVertex(1200, 1200, maze.rows, maze.cols);
    } else {
        rassert(false, 324289347238920081);
    }

    std::cout << "encodeVertex is done" << std::endl;
    const int INF = std::numeric_limits<int>::max();

    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута

    std::vector<int> distances(nvertices, INF);
    // TODO СКОПИРУЙТЕ СЮДА ДЕЙКСТРУ ИЗ ПРЕДЫДУЩЕГО ИСХОДНИКА

    // TODO в момент когда вершина становится обработанной - красьте ее на картинке window в зеленый цвет и показывайте картинку:
    //    cv::Point2i p = decodeVertex(the_chosen_one, maze.rows, maze.cols);
    //    window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
    //    cv::imshow("Maze", window);
    //    cv::waitKey(1);
    // TODO это может тормозить, в таком случае показывайте window только после обработки каждой сотой вершины

    // TODO обозначьте найденный маршрут красными пикселями

    // TODO сохраните картинку window на диск
    distances[start] = 0;
    std::vector<bool> isP(nvertices, false);
    std::vector<int> parents(nvertices, -1);
    // TODO ...

    bool fine = false, fin = true;

    int q = 0, w = 0;
    std::cout << "start maze" << std::endl;
    while (fin) {
        q++;
//        std::cout << 1 << std::endl;
        int nv = finish;
        for (int i = 0; i < distances.size(); ++i) {
            if (distances[i] < distances[nv] && !isP[i]){
                nv = i;
            }
        }
        if (nv == finish){
            if (w == 100) {
                fine = true;
                fin = false;
            }
            w++;
        }
//        std::cout << 1 << std::endl;
//        std::cout << nv << std::endl;
        if (nv == INF){
            fin = false;
        }
        for (int i = 0; i < edges_by_vertex[nv].size(); ++i) {
//            std::cout << edges_by_vertex[nv][i].u << " --> " << edges_by_vertex[nv][i].v << " " << distances[edges_by_vertex[nv][i].v] << " " << distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w << " " << parents[edges_by_vertex[nv][i].u] << std::endl;
            if (distances[edges_by_vertex[nv][i].v] > distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w){
                distances[edges_by_vertex[nv][i].v] = distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w;
                parents[edges_by_vertex[nv][i].v] = nv;
            }
            isP[edges_by_vertex[nv][i].u] = true;
        }
//        std::cout << 1 << std::endl;
        if (q % 10 == 0) {
            cv::Point2i p = decodeVertex(nv, maze.rows, maze.cols);
            window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
            cv::imshow("Maze", window);
            cv::waitKey(1);
        }
//        std::cout << 1 << std::endl;
    }
    std::cout << "fine maze" << std::endl;

    if (fine) {
        std::vector<int> path;
        int a = finish;
        path.push_back(a);
        while (a != start){
            a = parents[a];
            path.push_back(a);
        }
        for (int i = path.size()-1; i >= 0; i--) {
            std::cout << (path[i] + 1) << " ";
            cv::Point2i p = decodeVertex(path[i], maze.rows, maze.cols);
            window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(255, 0, 0);
            cv::imshow("Maze", window);
            cv::waitKey(1);
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }

    std::cout << "Finished!" << std::endl;

    // Показываем результат пока пользователь не насладиться до конца и не нажмет Escape
    while (cv::waitKey(10) != 27) {
        cv::imshow("Maze", window);
    }
}

int main() {
    try {
        int mazeNumber = 1;
        run(mazeNumber);

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
