#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <filesystem>


struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

// Эта биективная функция по координате пикселя (строчка и столбик) + размерам картинки = выдает номер вершины
int encodeVertex(int row, int column, int nrows, int ncolumns) {
    rassert(row < nrows, 348723894723980017);
    rassert(column < ncolumns, 347823974239870018);
    int vertexId = row * ncolumns + column;
    return vertexId;
}

// Эта биективная функция по номеру вершины говорит какой пиксель этой вершине соовтетствует (эта функция должна быть симметрична предыдущей!)
cv::Point2i decodeVertex(int vertexId, int nrows, int ncolumns) {

    // TODO: придумайте как найти номер строки и столбика пикселю по номеру вершины (просто поймите предыдущую функцию и эта функция не будет казаться сложной)
    int row = vertexId/ncolumns;
    int column = vertexId%ncolumns;

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

    int nvertices = maze.rows*maze.cols; // TODO

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            cv::Vec3b color = maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];
            int u = encodeVertex(j,i,maze.rows, maze.cols);
//            std::cout << i << " " << j << std::endl;
            if (i != 0){
                cv::Vec3b color1 = maze.at<cv::Vec3b>(j, i-1);
                int v = encodeVertex(j,i-1,maze.rows, maze.cols);
                int w = 1+nvertices*sqrt(((color1[0]-color[0])*(color1[0]-color[0])+(color1[1]-color[1])*(color1[1]-color[1])+(color1[2]-color[2])*(color1[2]-color[2]))/3);
                edges_by_vertex[u].push_back(Edge(u, v,w));
            }
            if (j != 0){
                cv::Vec3b color1 = maze.at<cv::Vec3b>(j-1, i);
                int v = encodeVertex(j-1,i,maze.rows, maze.cols);
                int w = 1+nvertices*sqrt(((color1[0]-color[0])*(color1[0]-color[0])+(color1[1]-color[1])*(color1[1]-color[1])+(color1[2]-color[2])*(color1[2]-color[2]))/3);
                edges_by_vertex[u].push_back(Edge(u, v,w));
            }
            if (i != maze.cols-1){
                cv::Vec3b color1 = maze.at<cv::Vec3b>(j, i+1);
                int v = encodeVertex(j,i+1,maze.rows, maze.cols);
                int w = 1+nvertices*sqrt(((color1[0]-color[0])*(color1[0]-color[0])+(color1[1]-color[1])*(color1[1]-color[1])+(color1[2]-color[2])*(color1[2]-color[2]))/3);
                edges_by_vertex[u].push_back(Edge(u, v,w));
            }
            if (j != maze.rows-1){
                cv::Vec3b color1 = maze.at<cv::Vec3b>(j+1, i);
                int v = encodeVertex(j+1,i,maze.rows, maze.cols);
                int w = 1+nvertices*sqrt(((color1[0]-color[0])*(color1[0]-color[0])+(color1[1]-color[1])*(color1[1]-color[1])+(color1[2]-color[2])*(color1[2]-color[2]))/3);
                edges_by_vertex[u].push_back(Edge(u, v,w));
            }

            // TODO добавьте соотвтетсвующие этому пикселю ребра
        }
    }
    std::cout << 1 << std::endl;

    int start, finish;
    if (mazeNumber >= 1 && mazeNumber <= 3) { // Первые три лабиринта очень похожи но кое чем отличаются...
        start = encodeVertex(300, 300, maze.rows, maze.cols);
        finish = encodeVertex(0, 305, maze.rows, maze.cols);
    } else if (mazeNumber == 4) {
        start = encodeVertex(154, 312, maze.rows, maze.cols);
        finish = encodeVertex(477, 312, maze.rows, maze.cols);
    } else if (mazeNumber == 5) { // Лабиринт в большом разрешении, добровольный (на случай если вы реализовали быструю Дейкстру с приоритетной очередью)
        start = encodeVertex(1200, 1200, maze.rows, maze.cols);
        finish = encodeVertex(1200, 1200, maze.rows, maze.cols);
    } else {
        rassert(false, 324289347238920081);
    }

    const int INF = std::numeric_limits<int>::max();

    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута

    std::vector<int> distances(nvertices, INF);
    distances[start] = 0;
    std::vector<bool> isP(nvertices, false);
    std::vector<int> parents(nvertices, -1);
    // TODO ...

    bool fine = false, fin = true;
    int q = 0;

    std::cout << 1 << std::endl;
    while (fin) {
        int nv = finish;
        for (int i = 0; i < distances.size(); ++i) {
            if (distances[i] <= distances[nv] && !isP[i]){
                nv = i;
            }
        }
        if (nv == finish){
            fine = true;
            q++;
            if (q == 5){
                fin = false;
            }
        }
        //        std::cout << nv << std::endl;
        if (nv == INF){
            fin = false;
        }
        for (int i = 0; i < edges_by_vertex[nv].size(); ++i) {
            //            std::cout << nv << " " << i << " " << edges_by_vertex[nv][i].v << " " << edges_by_vertex[nv][i].u << " " << distances[edges_by_vertex[nv][i].v] << " " << distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w << std::endl;
            if (distances[edges_by_vertex[nv][i].v] > distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w){
                distances[edges_by_vertex[nv][i].v] = distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w;
                parents[edges_by_vertex[nv][i].v] = nv;
                isP[nv] = true;
            }
        }
            cv::Point2i p = decodeVertex(nv, maze.rows, maze.cols);
            window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
            cv::imshow("Maze", window);
            cv::waitKey(1);
    }

    if (fine) {
        std::string resultsDir = "lesson15/resultsData/";
        if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
            std::filesystem::create_directory(resultsDir); // то создаем ее
        }

        cv::imwrite(resultsDir + "0img0.jpg", window);
    } else {
        std::cout << -1 << std::endl;
    }
    // TODO СКОПИРУЙТЕ СЮДА ДЕЙКСТРУ ИЗ ПРЕДЫДУЩЕГО ИСХОДНИКА

    // TODO в момент когда вершина становится обработанной - красьте ее на картинке window в зеленый цвет и показывайте картинку:
    //    cv::Point2i p = decodeVertex(the_chosen_one, maze.rows, maze.cols);
    //    window.at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(0, 255, 0);
    //    cv::imshow("Maze", window);
    //    cv::waitKey(1);
    // TODO это может тормозить, в таком случае показывайте window только после обработки каждой сотой вершины

    // TODO обозначьте найденный маршрут красными пикселями

    // TODO сохраните картинку window на диск

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
