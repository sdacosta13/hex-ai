#ifndef GameState_H
#define GameState_H

#include <vector>
#include <cmath>
#include <tuple>
#include "UnionFind.h"

const int BOARDSIZE = 11;
const int playerRED = 1;
const int playerBLUE = -1;
const std::tuple<int,int> EDGE1 = std::make_tuple(-1,-1);
const std::tuple<int,int> EDGE2 = std::make_tuple(-2,-2);
const std::vector<std::tuple<int,int>> neighbourTemplates = {std::tuple<int,int>(-1,0), std::tuple<int,int>(0,-1), std::tuple<int,int>(-1,1),
                                                             std::tuple<int,int>(0,1), std::tuple<int,int>(1,0), std::tuple<int,int>(1,-1)};

class GameState{
    public:
        GameState();
        void play(std::tuple<int,int> cell);
        int winner();
        int turn();
        bool would_lose(std::tuple<int, int> cell, int colour);
        void setTurn(int player);
        std::vector<std::tuple<int, int>> moves();
        std::vector<std::tuple<int, int>> getNeighbours(std::tuple<int, int> cell);
    private:
        int board[BOARDSIZE][BOARDSIZE] = {0};
        int to_play = playerRED;
        int red_played = 0;
        int blue_played = 0;      
        void play_red(std::tuple<int,int> cell);
        void play_blue(std::tuple<int,int> cell);
        UnionFind redGroups;
        UnionFind blueGroups;

};

#endif