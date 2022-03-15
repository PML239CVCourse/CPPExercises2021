#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <libutils/rasserts.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <queue>

const int INF = std::numeric_limits<int>::max();

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
    int row = vertexId / ncolumns;
    int column = vertexId % ncolumns;

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

    int nvertices = maze.cols * maze.rows; // TODO

    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int j = 0; j < maze.rows; ++j) {
        for (int i = 0; i < maze.cols; ++i) {
            cv::Vec3b color = maze.at<cv::Vec3b>(j, i);
            unsigned char blue = color[0];
            unsigned char green = color[1];
            unsigned char red = color[2];

            for(int itj = j - 1; itj <= j + 1; itj++){
                for(int iti = i - 1; iti <= i + 1; iti++){
                    if((itj == j - 1 && iti == i - 1) || (itj == j - 1 && iti == i + 1) ||
                            (itj == j + 1 && iti == i - 1) || (itj == j + 1 && iti == i + 1) || (iti == i && itj == j))
                        continue;

                    if((iti < 0) || (itj < 0) || (iti >= maze.cols) || (itj >= maze.rows))
                        continue;

                    cv::Vec3b color_it = maze.at<cv::Vec3b>(itj, iti);
                    unsigned char blue_it = color_it[0];
                    unsigned char green_it = color_it[1];
                    unsigned char red_it = color_it[2];

                    int w = 1;
                    if(!(blue == blue_it && red == red_it && green == green_it))
                        //w = INF;
                        continue;

                    int ai = encodeVertex(j, i, maze.rows, maze.cols);
                    int bi = encodeVertex(itj, iti, maze.rows, maze.cols);
                    edges_by_vertex[ai].push_back(Edge(ai, bi, w));
                    //edges_by_vertex[bi].push_back(Edge(bi, ai, w));

                }
            }
            //std::cout << edges_by_vertex[encodeVertex(j, i, maze.rows, maze.cols)].size() << " ";
            // TODO добавьте соотвтетсвующие этому пикселю ребра
        }
    }


    std::cout << "\n";

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


    cv::Mat window = maze.clone(); // на этой картинке будем визуализировать до куда сейчас дошла прокладка маршрута


    // TODO СКОПИРУЙТЕ СЮДА ДЕЙКСТРУ ИЗ ПРЕДЫДУЩЕГО ИСХОДНИКА

    std::vector<int> distances(nvertices, INF);
    distances[start] = 0;
    std::vector<int> p(nvertices, -1);

    /*std::vector <bool> used(nvertices);
    std::priority_queue<int> q;
    q.push(start);

    int cnt = 0;
    while (!q.empty()) {
        int v = q.top();
        q.pop();
        for(auto el: edges_by_vertex[v]){
            if(el.w + distances[v] < distances[el.v]){
                distances[el.v] = el.w + distances[v];
                p[el.v] = v;
                q.push(el.v);
            }
        }

        cnt++;
        //window.at<cv::Vec3b>(decodeVertex(v, maze.rows, maze.cols)) = cv::Vec3b(0, 255, 0);
        if(cnt % 1000 == 0){
            cnt = 0;
            cv::imshow("Maze", window);
                cv::waitKey(10);
        }
    }*/


    std::vector <bool> used(nvertices, false);
    std::priority_queue<int> q;
    q.push(start);
    used[start] = true;
    int cnt = 0;

    while (!q.empty()) {
        int v = q.top();
        q.pop();
        for(auto el: edges_by_vertex[v]){
            //if(used[el.v])
            //continue;
            if(el.w + distances[v] < distances[el.v]){
                distances[el.v] = el.w + distances[v];
                p[el.v] = v;
                q.push(el.v);
            }
        }

        cnt++;
        window.at<cv::Vec3b>(decodeVertex(v, maze.rows, maze.cols)) = cv::Vec3b(0, 255, 0);
        if(cnt % 1000 == 0){
            cnt = 0;
            cv::imshow("Maze", window);
            cv::waitKey(10);
        }
        //used[v] = true;
    }


    if (!(p[finish] == -1)) {
        std::vector<int> path;
        path.emplace_back(finish);
        int idx = finish;

        while(idx != start){
            idx = p[idx];
            path.emplace_back(idx);
        }
        std::reverse(path.begin(), path.end());

        /*for (auto it : path) {
            std::cout << (it + 1) << " ";
        }
        std::cout << std::endl;*/
        for(auto it : path){
            cv::Point2i p = decodeVertex(it, maze.rows, maze.cols);
            window.at<cv::Vec3b>(p) = cv::Vec3b(255, 0 , 0);
        }
        std::string path_saving = "lesson15/results/maze_path" + std::to_string(mazeNumber) + ".jpg";
        cv::imwrite(path_saving, window);
    }
    else {
        std::cout << "No path to this point" << std::endl;
        std::string path_saving = "lesson15/results/maze_path" + std::to_string(mazeNumber) + ".jpg";
        cv::imwrite(path_saving, window);
    }

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
        int mazeNumber = 3;
        run(mazeNumber);

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
