#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"

using namespace std;
DisjointSet::DisjointSet(int size)
{
    parent = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);
    //  - заполните вектора так чтобы на этапе конструирования эта система непересекающихся множеств состояла из:
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?)
    for(int i=0;i<size;++i) {parent[i]=ROOT;ranks[i]=0;sizes[i]=1;}
    //  заполнить parents
    //  заполнить ranks
    //  заполнить sizes
}

int	DisjointSet::get_set(int element)
{
    //  по номеру элемента нужно переходя по ссылкам на родителя дойти до самого верхнего элемента,
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
    vector <int> v;
    while(parent[element]!=(-1)) {v.push_back(element);element=parent[element];}
    for(auto h:v) parent[h]=element;
    return element;
}

int DisjointSet::count_differents() const
{
    //  сколько разных множеств (подсказка: в каждом множестве ровно один корень, а корень - это тот у кого родитель = ROOT)
    int count = 0;
    for (size_t i = 0; i < this->parent.size(); i++) {
        if(parent[i]==(-1)) ++count;
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
    //  сообщить сколько элементов в множестве, которому принадлежит данный элемент (да, это очень просто)
    return sizes[get_set(element)];
}

int	DisjointSet::union_sets(int element0, int element1)
{
    //  узнать корневые элементы этих двух элементов и посмотрев на них - решить,
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества
    element0=get_set(element0);element1=get_set(element1);
    if(ranks[element0]<ranks[element1]) {parent[element0]=element1;sizes[element1]+=sizes[element0];return element1;}
    else if(ranks[element0]==ranks[element1]) {parent[element0]=element1;++ranks[element0];sizes[element1]+=sizes[element0];return element1;}
    else {parent[element1]=element0;sizes[element0]+=sizes[element1];return element0;}
}