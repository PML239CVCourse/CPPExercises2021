#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

int debugPoint(int line) {
    if (line < 0)
        return 0;

    // You can put breakpoint at the following line to catch any rassert failure:
    return line;
}

#define rassert(condition, message) if (!(condition)) { std::stringstream ss; (ss << "Assertion \"" << message << "\" failed at line " << debugPoint(__LINE__) << "!"); throw std::runtime_error(ss.str()); }


struct Edge {
    int u, v; // номера вершин которые это ребро соединяет
    int w; // длина ребра (т.е. насколько длинный путь предстоит преодолеть переходя по этому ребру между вершинами)

    Edge(int u, int v, int w) : u(u), v(v), w(w)
    {}
};

void run() {
    // https://codeforces.com/problemset/problem/20/C?locale=ru
    // Не требуется сделать оптимально быструю версию, поэтому если вы получили:
    //
    // Превышено ограничение времени на тесте 31
    //
    // То все замечательно и вы молодец.

    int nvertices, medges;
    std::cin >> nvertices;
    std::cin >> medges;

    std::cout << "entered" << std::endl;
    std::vector<std::vector<Edge>> edges_by_vertex(nvertices);
    for (int i = 0; i < medges; ++i) {
        int ai, bi, w;
        std::cin >> ai >> bi >> w;
        rassert(ai >= 1 && ai <= nvertices, 23472894792020);
        rassert(bi >= 1 && bi <= nvertices, 23472894792021);

        ai -= 1;
        bi -= 1;
        rassert(ai >= 0 && ai < nvertices, 3472897424024);
        rassert(bi >= 0 && bi < nvertices, 3472897424025);

        Edge edgeAB(ai, bi, w);
        edges_by_vertex[ai].push_back(edgeAB);

        edges_by_vertex[bi].push_back(Edge(bi, ai, w)); // а тут - обратное ребро, можно конструировать объект прямо в той же строчке где он и потребовался
    }
    std::cout << "edges completed" << std::endl;

    const int start = 0;
    const int finish = nvertices - 1;

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> distances(nvertices, INF);
    distances[start] = 0;
    std::vector<bool> isP(nvertices, false);
    std::vector<int> parents(nvertices, -1);
    // TODO ...

    bool fine = false;

    while (true) {
        int nv = finish;
        for (int i = 0; i < distances.size(); ++i) {
            if (distances[i] <= distances[nv] && !isP[i]){
                nv = i;
            }
        }
        if (nv == finish){
            fine = true;
        }
        std::cout << nv << std::endl;
        if (nv == INF){
            break;
        }
        for (int i = 0; i < edges_by_vertex[nv].size(); ++i) {
            std::cout << nv << " " << i << " " << edges_by_vertex[nv][i].v << " " << edges_by_vertex[nv][i].u << " " << distances[edges_by_vertex[nv][i].v] << " " << distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w << std::endl;
            if (distances[edges_by_vertex[nv][i].v] > distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w){
                distances[edges_by_vertex[nv][i].v] = distances[edges_by_vertex[nv][i].u] + edges_by_vertex[nv][i].w;
                parents[edges_by_vertex[nv][i].v] = nv;
                isP[i] = true;
            }
        }
    }

    if (fine) {
        std::vector<int> path;
        int a = finish;
        path.push_back(a);
        while (a != start){
            a = parents[a];
            path.push_back(a);
        }
        for (int i = path.size(); i > 0; i--) {
            std::cout << (path[i] + 1) << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << -1 << std::endl;
    }
}

int main() {
    try {
        run();

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}
