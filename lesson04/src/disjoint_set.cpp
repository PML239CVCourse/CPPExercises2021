#include "disjoint_set.h"

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);
    for(auto &i: parents)
        i = -1;
    for(auto &i: ranks)
        i = 0;
    for(auto &i: sizes)
        i = 1;
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?
}

int	DisjointSet::get_set(int element)
{
    if(parents[element]==-1)
        return element;
    return parents[element] = get_set(parents[element]);
}

int DisjointSet::count_differents() const
{
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); i++) {
        if(parents[i]==-1)
            count++;
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
    return sizes[get_set(element)];
}

int	DisjointSet::union_sets(int element0, int element1)
{
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества
    int A = get_set(element0);
    int B = get_set(element1);
    if(ranks[A]<ranks[B])
        std::swap(A,B);
    if(ranks[A]==ranks[B])
        ranks[A]++;
    parents[B]=A;
    sizes[A]+= sizes[B];
    return A;
}
