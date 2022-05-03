#include "disjoint_set.h"

#include <iostream>

#include <libutils/rasserts.h>

#define ROOT -1 // объявили макрос (считайте константу) равный минус единице - чтобы не было "волшебной переменной", а была именованная константа "корень дерева"


DisjointSet::DisjointSet(int size) {
    parents = std::vector<int>(size, ROOT);
    ranks = std::vector<int>(size, 0);
    sizes = std::vector<int>(size, 1);
}

int DisjointSet::size() const {
    return parents.size();
}

int DisjointSet::get_set(int element) {
    int parent = parents[element];
    if (parent != ROOT) {
        int set_super_parent = get_set(parent);
        if (set_super_parent != element) {
            parents[element] = set_super_parent;
        }
        return set_super_parent;
    } else {
        return element;
    }
}

int DisjointSet::count_differents() const {
    int count = 0;
    for (size_t i = 0; i < this->parents.size(); ++i) {
        if (parents[i] == ROOT) {
            ++count;
        }
    }
    return count;
}

int DisjointSet::get_set_size(int element) {
    return sizes[get_set(element)];
}

int DisjointSet::union_sets(int element0, int element1) {
    int parent0 = get_set(element0);
    int parent1 = get_set(element1);
    rassert(parent0 != parent1, 34718927410047);
    if (this->ranks[parent0] == this->ranks[parent1]) {
        this->parents[parent1] = parent0;
        this->ranks[parent0]++;
        this->sizes[parent0] += this->sizes[parent1];
        return parent0;
    } else if (this->ranks[parent0] > this->ranks[parent1]) {
        this->parents[parent1] = parent0;
        this->sizes[parent0] += this->sizes[parent1];
        return parent0;
    } else {
        this->parents[parent0] = parent1;
        this->sizes[parent1] += this->sizes[parent0];
        return parent1;
    }
}