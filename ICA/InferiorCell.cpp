#include <iostream>
#include <cstring>
#include <time.h>
#include <climits>
#include <tuple>
using namespace std;


#define BOARD_WIDTH 5
#define USE_COLOR true
// Unsure of what the states are represented as
//#define PLAYER_1_COLOUR 1
//#define PLAYER_2_COLOUR -1
//#define EMPTY_COLOUR 0

class Board{
  public:
    enum Tile {
      PLAYER_RED = 1, // Red
      PLAYER_BLUE = -1, // Blue
      EMPTY_COLOUR = 0
    };
    Tile board [BOARD_WIDTH][BOARD_WIDTH] = {};
    void SetBoard(int x, int y, Tile value){
      board[y][x] = value;
    }
    Tile GetBoard(int x, int y){
      return board[y][x];
    }

    void PrintBoard(){
      for(int y = 0; y < BOARD_WIDTH; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
          if(GetBoard(x, y) == PLAYER_RED){
            if(USE_COLOR){
              cout << "\e[1;31mR\e[0m";
            } else {
              cout << 'R';
            }

          } else if (GetBoard(x, y) == PLAYER_BLUE) {
            if(USE_COLOR){
              cout << "\e[1;34mB\e[0m";
            } else {
              cout << 'B';
            }
          } else if(GetBoard(x, y) == EMPTY_COLOUR){
            if(USE_COLOR){
              cout << "\e[1;37m0\e[0m";
            } else {
              cout << '0';
            }
          }
          cout << ' ';
        }
        cout << "\n";
        if(y + 1 != BOARD_WIDTH){
          for(int i = 0; i < y + 1; i++){
            cout << ' ';
          }
        }
      }
    }
    // [pattern number][Red Or Blue][Coordinate pair][Coordinate]
    #define PATTERN_TOT 3
    #define PATTERN_MAX_COORDS 4
    #define X -100
    const int patterns [PATTERN_TOT][2][PATTERN_MAX_COORDS][2] = {
      {
        {{-1, 0}, {0,-1}, {1,-1}, {1, 0}},
        {{ X, X}, {X, X}, {X, X}, {X, X}}
      },
      {
        {{0, -1}, {1, -1}, {1, 0}, {X, X}},
        {{-1, 1}, {X,  X}, {X, X}, {X, X}}
      },
      {
        {{1,-1}, {1, 0}, {X, X}, {X, X}},
        {{-1,0}, {-1,1}, {X, X}, {X, X}}
      }
    };
    const int pattern_rotation_inner_ring [6][2] = {
      {0, -1},
      {1, -1},
      {1,  0},
      {0,  1},
      {-1, 1},
      {-1, 0}
    };
    const int pattern_rotation_outer_ring [12][2] = {
      {2,  -2},
      {2,  -1},
      {2,   0},
      {1,   1},
      {0,   2},
      {-1,  2},
      {-2,  2},
      {-2,  1},
      {-2,  0},
      {-1, -1},
      {0,  -2},
      {1,  -2}
    };



    tuple<bool, int> CheckForDeadCell(int x, int y){
      int colourSel1 = 0;
      int colourSel2 = 1;
      for(int colourNum = 0; colourNum < 2; colourNum++){
        for(int patternIndex = 0; patternIndex < PATTERN_TOT; patternIndex++){

          bool patternMatched = true;
          for(int redCoord = 0; redCoord < PATTERN_MAX_COORDS; redCoord++){
            if(patterns[patternIndex][colourSel1][redCoord][0] != X){
              Tile res = GetBoard(x + patterns[patternIndex][colourSel1][redCoord][0], y + patterns[patternIndex][colourSel1][redCoord][1]);
              if(res != PLAYER_RED){
                patternMatched = false;
              }
            }
          }

          for(int blueCoord = 0; blueCoord < PATTERN_MAX_COORDS; blueCoord++){
            if(patterns[patternIndex][colourSel2][blueCoord][0] != X){
              Tile res = GetBoard(x + patterns[patternIndex][colourSel2][blueCoord][0], y + patterns[patternIndex][colourSel2][blueCoord][1]);
              if(res != PLAYER_BLUE){
                patternMatched = false;
              }
            }
          }
          if(patternMatched){
            return make_tuple(true, patternIndex);
          }




        }
        colourSel1 = 1;
        colourSel2 = 0;
      }

      return make_tuple(false, 0);
    }
    Board(bool random){
      for(int y = 0; y < BOARD_WIDTH; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
          if(random){
            int r = rand() % 3; // Just for testing purposes fill the board with random positions
            if(r == 0){
              SetBoard(x, y, PLAYER_RED);
            } else if(r == 1) {
              SetBoard(x, y, PLAYER_BLUE);
            } else {
              SetBoard(x, y, EMPTY_COLOUR);
            }
          } else {
            SetBoard(x,y, EMPTY_COLOUR);
          }
        }
      }
    }

};


int main() {
  Board b(false);
  int x = 2;
  int y = 2;
  b.SetBoard(x-1, y, Board::PLAYER_BLUE);
  b.SetBoard(x,   y-1,Board::PLAYER_BLUE);
  b.SetBoard(x+1, y-1, Board::PLAYER_BLUE);
  b.SetBoard(x+1, y, Board::PLAYER_BLUE);
  clock_t before = clock();
  for(int y = 0; y < BOARD_WIDTH; y++){
   for(int x = 0; x < BOARD_WIDTH; x++){
     tuple<bool, int> result = b.CheckForDeadCell(x,y);
     if(get<0>(result)){
       cout << "Dead Cell found at: (" << x << ", " << y << ") Rule used: " << get<1>(result) << endl;
     }
   }
  }
  clock_t diff = clock() - before;
  int msec = diff * 1000/ CLOCKS_PER_SEC;
  b.PrintBoard();
  cout << "Time taken: " << msec << endl;
  return 0;
};
