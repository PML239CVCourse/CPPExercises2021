#include "disjoint_set.h"
#include <libutils/rasserts.h>

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size)
{
    parents = std::vector<int>(size);
    ranks = std::vector<int>(size);
    sizes = std::vector<int>(size);
    // TODO - заполните вектора так чтобы на этапе конструирования эта система непересекающихся множеств состояла из:
    // size элементов, каждый из которых состоит в своем множестве (а значит ссылка на родителя у него - на корень, т.е. на ROOT, а какой тогда ранк и размер у множества каждого элемента?)
    // TODO заполнить parents
    for (int i = 0; i < size; ++i) {
        parents[i] = -1;
    }
    // TODO заполнить ranks
    for (int i = 0; i < size; ++i) {
        ranks[i] = 0;
    }
    // TODO заполнить sizes
    for (int i = 0; i < size; ++i) {
        sizes[i] = 1;
    }
}

int	DisjointSet::get_set(int element)
{
    // TODO по номеру элемента нужно переходя по ссылкам на родителя дойти до самого верхнего элемента,
    // номер этого корневого элемента - номер множества на данный момент (кто вверху тот и главный, множество названо в его честь)
    int first_of_their_kind = 0;
    if ( parents[element] != element && parents[element] != -1){
        parents[element] = get_set(parents[element]);
    } else{
        first_of_their_kind = element;
    }
    return first_of_their_kind;
}

int DisjointSet::count_differents() const
{
    // TODO посчитать сколько разных множеств (подсказка: в каждом множестве ровно один корень, а корень - это тот у кого родитель = ROOT)
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); i++) {
        if (parents[i] == -1) count++;
    }
    return count;
}

int DisjointSet::get_set_size(int element)
{
    int count = 0;
    // TODO сообщить сколько элементов в множестве, которому принадлежит данный элемент (да, это очень просто)
    int root = get_set(element);
    for (int i = 0; i < this->parents.size(); ++i) {
        if (get_set(i) == root){
            count++;
        }
    }
    return count;
}

int	DisjointSet::union_sets(int element0, int element1)
{
    // TODO узнать корневые элементы этих двух элементов и посмотрев на них - решить,
    // кого из них подвесить к другому (тем самым объединить два множества)
    // при этом стоит подвешивать менее высокое дерево к более высокому (т.е. учитывая ранк),
    // а так же важно не забыть после переподвешивания у корневого элемента обновить ранк и размер множества
    element0 = get_set(element0);
    element1 = get_set(element1);

    int number = 0;
    if(element0 == element1)
        return element1;
    if(ranks[element0] == ranks[element1]){
        ranks[element0]++;
    }
    if (ranks[element0] < ranks[element1]){
        parents[element0] = element1;
        number = element1;
    }else{
        parents[element1] = element0;
        number = element0;
    }
    return number; // вернуть номер объединенного множества
}
