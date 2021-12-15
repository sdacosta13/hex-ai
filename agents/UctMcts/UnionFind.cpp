#include <iostream>
#include "UnionFind.h"
#include <algorithm>

UnionFind::UnionFind(){
    
    std::vector<int> ignored;
}

bool UnionFind::join(std::tuple<int,int> x, std::tuple<int,int> y){
    std::tuple<int,int> rep_x = this->find(x);
    std::tuple<int,int> rep_y = this->find(y);
    if (rep_x == rep_y){
        return false;
    };
    if (this->rank[rep_x] < this->rank[rep_y]){
        this->parent[rep_x] = rep_y;
        this->groups[rep_y].insert(this->groups[rep_y].end(), this->groups[rep_x].begin(),this->groups[rep_x].end());
        this->groups.erase(rep_x);
    }else if (this->rank[rep_x] > this->rank[rep_y]){
        this->parent[rep_y] = rep_x;
        this->groups[rep_x].insert(this->groups[rep_x].end(), this->groups[rep_y].begin(),this->groups[rep_y].end());
        this->groups.erase(rep_y);
    }else {
        this->parent[rep_x] == rep_y;
        this->rank[rep_y] += 1;
        this->groups[rep_y].insert(this->groups[rep_y].end(), this->groups[rep_x].begin(),this->groups[rep_x].end());
        this->groups.erase(rep_x);
    };
    return true;
};

std::tuple<int,int> UnionFind::find(std::tuple<int,int> x){
    if (this->parent.count(x) == 0){
        this->parent[x] = x;
        this->rank[x] = 0;
        if (std::find(this->ignored.begin(), this->ignored.end(), x) != this->ignored.end()){
            this->groups[x].clear();
        } else {
            this->groups[x] = {x};
        }
    }

    std::tuple<int,int> px = this->parent[x];
    if (x == px){
        return x;
    }

    std::tuple<int,int> gx = this->parent[px];
    if (gx == px){
        return px;
    }

    this->parent[x] = gx;

    return this->find(gx);
};

bool UnionFind::connected(std::tuple<int,int> x, std::tuple<int,int> y) {
    return (this->find(x) == this->find(y));
};

void UnionFind::set_ignored(std::vector<std::tuple<int,int>> ignoreList) {
    this->ignored = ignoreList;
}

std::map<std::tuple<int,int>, std::vector<std::tuple<int,int>>> UnionFind::getGroups(){
    return this->groups;
}

