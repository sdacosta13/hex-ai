#ifndef UnionFind_H
#define UnionFind_H
#include <map>
#include <vector>
#include <tuple>

class UnionFind{
    public:
        UnionFind();
        void set_ignored(std::vector<std::tuple<int,int>> ignoreList);
        bool join(std::tuple<int,int> x, std::tuple<int,int> y);
        bool connected(std::tuple<int,int> x, std::tuple<int,int> y);
    private:
        std::tuple<int,int> find(std::tuple<int,int> x);
        std::map<std::tuple<int,int>, int> rank;
        std::map<std::tuple<int,int>,std::tuple<int,int>> parent;
        std::map<std::tuple<int,int>, std::vector<std::tuple<int,int>>> groups;
        std::vector<std::tuple<int,int>> ignored;
        std::map<std::tuple<int,int>, std::vector<std::tuple<int,int>>> getGroups();

};

#endif