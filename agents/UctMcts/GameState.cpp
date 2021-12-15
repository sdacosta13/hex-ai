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
        this->blueGroups.join(EDGE1, cell);
    }

    if (col == BOARDSIZE-1) {
        this->blueGroups.join(EDGE2, cell);
    }

    std::vector<std::tuple<int, int>> neighbours = this->getNeighbours(cell);
    for (auto itr:neighbours){
        if (this->board[std::get<0>(itr)][std::get<1>(itr)] == playerBLUE){
            this->blueGroups.join(itr, cell);
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
    if (this->redGroups.connected(EDGE1, EDGE2)){
        return playerRED;
    } else if (this->blueGroups.connected(EDGE1, EDGE2)){
        return playerBLUE;
    } else {
        return 0;
    }
}

void GameState::swap(){
  this->to_play = playerBLUE;
  UnionFind temp = this->redGroups;
  this->redGroups = this->blueGroups;
  this->blueGroups = this->redGroups;
}

std::vector<std::tuple<int, int>> GameState::moves() {
    std::vector<std::tuple<int, int>> moveList;
    for (int i=0; i<BOARDSIZE; i++) {
        for (int j=0; j<BOARDSIZE; j++){
            if (this->board[i][j] == 0) {
                moveList.push_back(std::tuple<int,int>(i,j));
            } else {
                std::cout << i << " " << j << "\n";
            }
        }
    }

    return moveList;
}
std::vector<std::tuple<int, int>> GameState::reducedMoves() {
    std::vector<std::tuple<int, int>> moveList;
    for (int i=0; i<BOARDSIZE; i++) {
        for (int j=0; j<BOARDSIZE; j++){
            if (this->board[i][j] == 0) {
                std::tuple<bool, int> res = CheckForDeadCell(i,j);
                if(!std::get<0>(res)){ // If pattern seen, discard move.
                    moveList.push_back(std::tuple<int,int>(i,j));
                }
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
int GameState::GetBoard(int x, int y){
  return this->board[y][x];
}
#define PATTERN_TOT 3 * 6
#define PATTERN_MAX_COORDS 4
#define X -100
const int patterns [PATTERN_TOT][2][PATTERN_MAX_COORDS][2] = {
  // First Pattern rotated
  {
    {{-1, 0}, {0,-1}, {1,-1}, {1, 0}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  {
    {{0, -1}, {1,-1}, {1, 0}, {0, 1}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  {
    {{1, -1}, {1, 0}, {0, 1}, {-1, 1}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  {
    {{1, 0}, {0, 1}, {-1, 1}, {-1, 0}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  {
    {{0, 1}, {-1, 1}, {-1, 0}, {0, -1}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  {
    {{-1, 1}, {-1, 0}, {0, -1}, {1, -1}},
    {{ X, X}, {X, X}, {X, X}, {X, X}}
  },
  // Second Pattern rotated
  {
    {{0, -1}, {1, -1}, {1, 0}, {X, X}},
    {{-1, 1}, {X,  X}, {X, X}, {X, X}}
  },
  {
    {{1, -1}, {1, 0}, {0, 1}, {X, X}},
    {{-1, 0}, {X,  X}, {X, X}, {X, X}}
  },
  {
    {{1, 0},  {0, 1}, {-1, 1}, {X, X}},
    {{0, -1}, {X,  X}, {X, X}, {X, X}}
  },
  {
    {{0, 1}, {-1, 1}, {-1, 0}, {X, X}},
    {{1, -1}, {X,  X}, {X, X}, {X, X}}
  },
  {
    {{-1, 1}, {-1, 0}, {0, -1}, {X, X}},
    {{1,  0}, {X,  X}, {X, X}, {X, X}}
  },
  {
    {{-1, 0}, {0, -1}, {1, -1}, {X, X}},
    {{0,  1}, {X,  X}, {X, X}, {X, X}}
  },
  // Third Pattern Rotated
  {
    {{1,-1}, {1, 0}, {X, X}, {X, X}},
    {{-1,0}, {-1,1}, {X, X}, {X, X}}
  },
  {
    {{1, 0}, {0, 1}, {X, X}, {X, X}},
    {{-1,0}, {0, -1}, {X, X}, {X, X}}
  },
  {
    {{0, 1}, {-1, 1}, {X, X}, {X, X}},
    {{0, -1}, {1, -1}, {X, X}, {X, X}}
  },
  {
    {{-1, 1}, {-1, 0}, {X, X}, {X, X}},
    {{1, -1}, {1, 0}, {X, X}, {X, X}}
  },
  {
    {{-1, 0}, {0, -1}, {X, X}, {X, X}},
    {{1, 0}, {0, 1}, {X, X}, {X, X}}
  },
  {
    {{0,-1}, {1, -1}, {X, X}, {X, X}},
    {{0, 1}, {-1, 1}, {X, X}, {X, X}}
  }
};
#define EDGE_TOT 6
#define EDGE_MAX_COORDS 2
const int edge_pattern [4][EDGE_TOT][2][EDGE_MAX_COORDS][2] = {
  {  // Bottom Edge
    { // pattern 1
      {{-1, 0}, {0, -1}},
      {{X,  X},  {X, X}}
    },
    { // pattern 1 flipped
      {{1, -1}, {1, 0}},
      {{X,  X},  {X, X}}
    },
    { // pattern 2
      {{-1, 0}, {1, 0}},
      {{X,  X}, {X, X}}
    },
    { // pattern 3
      {{1,  0}, {X, X}},
      {{0, -1}, {X, X}}
    },
    { // pattern 3 flipped
      {{-1, 0}, {X, X}},
      {{1,- 1}, {X, X}}
    },
    { // pattern 4
      {{X, X}, {X, X}},
      {{0, -1}, {1, -1}}
    }
  },
  {  // Top Edge
    { // pattern 1
      {{-1, 1}, {-1, 0}},
      {{X, X}, {X, X}}
    },
    { // pattern 1 flipped
      {{0, 1}, {1, 0}},
      {{X, X}, {X, X}}
    },
    { // pattern 2
      {{-1, 0}, {1, 0}},
      {{X, X}, {X, X}}
    },
    { // pattern 3
      {{-1, 0}, {X, X}},
      {{0, 1}, {X, X}}
    },
    { // pattern 3 flipped
      {{1, 0}, {X, X}},
      {{-1, 1}, {X, X}}
    },
    { // pattern 4
      {{X, X}, {X, X}},
      {{-1, 1}, {0, 1}}
    }
  },
  {  // Left Edge
    { // pattern 1
      {{X, X}, {X, X}},
      {{0, -1}, {1, -1}}
    },
    { // pattern 1 flipped
      {{X, X}, {X, X}},
      {{1, 0}, {0, 1}}
    },
    { // pattern 2
      {{X, X}, {X, X}},
      {{0, -1}, {0, 1}}
    },
    { // pattern 3
      {{1, -1}, {X, X}},
      {{0,  1}, {X, X}}
    },
    { // pattern 3 flipped
      {{1, 0}, {X, X}},
      {{0, -1}, {X, X}}
    },
    { // pattern 4
      {{1,-1}, {1, 0}},
      {{X, X}, {X, X}}
    }
  },
  {  // Right Edge
    { // pattern 1
      {{X, X}, {X, X}},
      {{0, -1}, {-1, 0}}
    },
    { // pattern 1 flipped
      {{X, X}, {X, X}},
      {{-1, 1}, {0, 1}}
    },
    { // pattern 2
      {{X, X}, {X, X}},
      {{0,-1}, {0, 1}}
    },
    { // pattern 3
      {{-1, 1}, {X, X}},
      {{0,-1}, {X, X}}
    },
    { // pattern 3 flipped
      {{-1, 0}, {X, X}},
      {{0, 1}, {X, X}}
    },
    { // pattern 4
      {{X, X}, {X, X}},
      {{-1, 1}, {-1, 0}}
    }
  }
};


std::tuple<bool, int> GameState::CheckCorner(int x, int y){ // Performs pattern recognition around the edges
  int edgenum;
  int colourA = playerRED;
  int colourB = playerBLUE;
  if((x == 0 && y == 0) || (x == 0 && y == BOARDSIZE-1) || (x == BOARDSIZE-1 && y == 0) || (x == BOARDSIZE- 1 && y == BOARDSIZE-1)){
    return std::make_tuple(false, 0);
  } else if(x == 0){
    edgenum = 2;
  } else if(x == BOARDSIZE-1){
    edgenum = 3;
  } else if(y == 0){
    edgenum = 1;
  } else if(y == BOARDSIZE-1){
    edgenum = 0;
  }
  for(int pIndex = 0; pIndex < EDGE_TOT; pIndex++){
    bool patternMatched = true;
    for(int c = 0; c < 2; c++){
      for(int coord = 0 ; coord < EDGE_MAX_COORDS; coord++){
        if(edge_pattern[edgenum][pIndex][c][coord][0] != X){
          int xCheck = x + edge_pattern[edgenum][pIndex][c][coord][0];
          int yCheck = y + edge_pattern[edgenum][pIndex][c][coord][1];
          int res = GetBoard(xCheck, yCheck);
          if(c == 0){
            if(res != colourA) patternMatched = false;
          } else {
            if(res != colourB) patternMatched = false;
          }
        }
      }
    }
    if(patternMatched){
      return std::make_tuple(true, pIndex);
    }
  }
  return std::make_tuple(false, 0);
}
std::tuple<bool, int> GameState::CheckForDeadCell(int x, int y){ // Obviously dont run this on non empty cells
  if(x != 0 && x != BOARDSIZE-1 && y != 0 && y != BOARDSIZE-1){
    int colourSel1 = 0;
    int colourSel2 = 1;
    for(int colourNum = 0; colourNum < 2; colourNum++){
      for(int patternIndex = 0; patternIndex < PATTERN_TOT; patternIndex++){

        bool patternMatched = true;
        for(int redCoord = 0; redCoord < PATTERN_MAX_COORDS; redCoord++){
          if(patterns[patternIndex][colourSel1][redCoord][0] != X){
            int xToCheck = x + patterns[patternIndex][colourSel1][redCoord][0];
            int yToCheck = y + patterns[patternIndex][colourSel1][redCoord][1];
            if(!(xToCheck < 0 || yToCheck < 0 || xToCheck >= BOARDSIZE || yToCheck >= BOARDSIZE)){
              int res = GetBoard(xToCheck, yToCheck);
              if(res != playerRED){
                patternMatched = false;
                break; // For Performance stop checking
              }
            } else {
              // When Culling against the edge of the board, I assume the pattern is not detected.
              // Therefore it is possible that this function will return false for a cell which is not Dead
              // I decided this was preferable to reporting true when the pixel was not dead
              patternMatched = false;
              break;
            }
          }
        }
        if(patternMatched){ // If devation from the pattern has been seen dont bother continuing checks
          for(int blueCoord = 0; blueCoord < PATTERN_MAX_COORDS; blueCoord++){
            if(patterns[patternIndex][colourSel2][blueCoord][0] != X){
              int xToCheck = x + patterns[patternIndex][colourSel2][blueCoord][0];
              int yToCheck = y + patterns[patternIndex][colourSel2][blueCoord][1];
              if(!(xToCheck < 0 || yToCheck < 0 || xToCheck >= BOARDSIZE || yToCheck >= BOARDSIZE)){
                int res = GetBoard(xToCheck, yToCheck);
                if(res != playerBLUE){
                  patternMatched = false;
                  break;
                }
              } else {
                patternMatched = false;
                break;
              }
            }
          }
        }
        if(patternMatched){
          return std::make_tuple(true, patternIndex);
        }

      }
      colourSel1 = 1;
      colourSel2 = 0;
    }
    return std::make_tuple(false, 0);
  } else {
    return CheckCorner(x,y);
  }
}
