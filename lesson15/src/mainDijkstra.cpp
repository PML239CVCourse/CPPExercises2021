#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <queue>

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

    const int start = 0;
    const int finish = nvertices - 1;

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> distances(nvertices, INF);
    distances[start] = 0;
    std::vector<int> p(nvertices, -1);
    // TODO ...

    std::vector <bool> used(nvertices, false);
    std::priority_queue<int> q;
    q.push(start);
    used[start] = true;

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
        //used[v] = true;
    }

    /*for(auto it : p)
        std::cout << it << " ";
    std::cout << "\n";*/
    if (!(p[finish] == -1)) {
        std::vector<int> path;
        path.emplace_back(finish);
        int idx = finish;

        while(idx != start){
            idx = p[idx];
            path.emplace_back(idx);
        }
        std::reverse(path.begin(), path.end());

        for (auto it : path) {
            std::cout << (it + 1) << " ";
        }
        std::cout << std::endl;
    }
    else {
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
