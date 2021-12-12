#include <iostream>
#include "GameState.h"

GameState::GameState(){
    redGroups.set_ignored(std::vector<std::tuple<int,int>>{EDGE1,EDGE2});
    blueGroups.set_ignored(std::vector<std::tuple<int,int>>{EDGE1,EDGE2});
}

void GameState::play(std::tuple<int,int> cell){
    if ((this->to_play) == playerRED) {
        this->play_red(cell);
        this->to_play = playerBLUE;
    } else {
        this->play_blue(cell);
        this->to_play = playerRED;
    }
};

void GameState::play_red(std::tuple<int,int> cell){
    int row = std::get<0>(cell);
    int col = std::get<1>(cell);
    if ((this->board)[row][col] == 0) {
        (this->board)[row][col] = playerRED;
        this->red_played += 1;
    } else {
        //Raise Error here
        std::cout << "Cell is already Occupied";
    }

    if (row == 0){
        this->redGroups.join(EDGE1, cell);
    } 

    if (row == BOARDSIZE-1) {
        this->redGroups.join(EDGE2, cell);
    }

    std::vector<std::tuple<int, int>> neighbours = this->getNeighbours(cell);
    for (auto itr:neighbours){
        if (this->board[std::get<0>(itr)][std::get<1>(itr)] == playerRED){
            this->redGroups.join(itr, cell);
        }
    }


}

void GameState::play_blue(std::tuple<int,int> cell){
    int row = std::get<0>(cell);
    int col = std::get<1>(cell);
    if ((this->board)[row][col] == 0) {
        (this->board)[row][col] = playerBLUE;
        this->red_played += 1;
    } else {
        //Raise Error here
        std::cout << "Cell is already Occupied";
    }

    if (col == 0){
        this->redGroups.join(EDGE1, cell);
    } 

    if (col == BOARDSIZE-1) {
        this->redGroups.join(EDGE2, cell);
    }

    std::vector<std::tuple<int, int>> neighbours = this->getNeighbours(cell);
    for (auto itr:neighbours){
        if (this->board[std::get<0>(itr)][std::get<1>(itr)] == playerBLUE){
            this->redGroups.join(itr, cell);
        }
    }
}

int GameState::turn(){
    return this->to_play;
}

void GameState::setTurn(int player) {
    if (player == playerRED || player == playerBLUE) {
        this->to_play = player;
    };
}

int GameState::winner(){
    if (redGroups.connected(EDGE1, EDGE2)){
        return playerRED;
    } else if (redGroups.connected(EDGE1, EDGE2)){
        return playerBLUE;
    } else {
        return 0;
    }
}

std::vector<std::tuple<int, int>> GameState::moves() {
    std::vector<std::tuple<int, int>> moveList;
    for (int i=0; i<BOARDSIZE; i++) {
        for (int j=0; j<BOARDSIZE; j++){
            if (this->board[i][j] == 0) {
                moveList.push_back(std::tuple<int,int>(i,j));
            }
        }
    }

    return moveList;
}

std::vector<std::tuple<int, int>> GameState::getNeighbours(std::tuple<int, int> cell){
    int r = std::get<0>(cell);
    int c = std::get<1>(cell);
    std::vector<std::tuple<int, int>> neighbourList;
    for (auto itr : neighbourTemplates){
        int nr = std::get<0>(itr);
        int nc = std::get<1>(itr);

        if ((0 <= nr + r < BOARDSIZE) && (0 <= c + nc < BOARDSIZE)){
            neighbourList.push_back(std::tuple<int,int>(nr,nc));
        }
    };

    return neighbourList;

}

bool GameState::would_lose(std::tuple<int, int> cell, int colour){
    bool connect1 = false;
    bool connect2 = false;
    int r = std::get<0>(cell);
    int c = std::get<1>(cell);
    std::vector<std::tuple<int, int>> neighbours = this->getNeighbours(cell);
    if (colour == playerBLUE){
        if (c == 0){
            connect1 = true;
        } else if (c == BOARDSIZE - 1){
            connect2 = true;
        }

        for (auto itr:neighbours){
            if (this->blueGroups.connected(EDGE1, itr)){
                connect1 = true;
            } else if (this->blueGroups.connected(EDGE2, itr)){
                connect2 = true;
            }
    }
    if (colour == playerRED){
        if (r == 0){
            connect1 = true;
        } else if (r == BOARDSIZE - 1){
            connect2 = true;
        }

        for (auto itr:neighbours){
            if (this->redGroups.connected(EDGE1, itr)){
                connect1 = true;
            } else if (this->redGroups.connected(EDGE2, itr)){
                connect2 = true;
            }
        }
    }

    return (connect1 && connect2);
}