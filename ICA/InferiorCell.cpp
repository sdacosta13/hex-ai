#include <iostream>
#include <cstring>
#include <chrono>
#include <climits>
#include <tuple>
using namespace std;
using namespace std::chrono;

#define BOARD_WIDTH 13
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
      for(int y = -1; y < BOARD_WIDTH+1; y++){
        for(int x = -1; x < BOARD_WIDTH+1; x++){
          if((y == -1 && x == -1) || (y == -1 && x == BOARD_WIDTH) || (y == BOARD_WIDTH && x == -1) || (y == BOARD_WIDTH && x == BOARD_WIDTH)){
            cout << ' ';
            if(y != -1){
              for (int i = 0; i < BOARD_WIDTH; i++){
                cout << ' ';
              }
            }
            continue;
          }
          else if(x == -1 || x == BOARD_WIDTH){
            if(USE_COLOR){
              cout << "\e[1;34m# \e[0m";
            } else {
              cout << 'B';
            }
            continue;
          }
          else if(y == -1 || y == BOARD_WIDTH){
            if(USE_COLOR){
              cout << "\e[1;31m# \e[0m";
            } else {
              cout << 'R';
            }
            continue;
          }
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
      cout << endl;
    }
    // [pattern number][Red Or Blue][Coordinate pair][Coordinate]
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

    tuple<bool, int> CheckCorner(int x, int y){ // Performs pattern recognition around the edges
      int edgenum;
      Tile colourA = PLAYER_RED;
      Tile colourB = PLAYER_BLUE;
      if((x == 0 && y == 0) || (x == 0 && y == BOARD_WIDTH-1) || (x == BOARD_WIDTH-1 && y == 0) || (x == BOARD_WIDTH- 1 && y == BOARD_WIDTH-1)){
        return make_tuple(false, 0);
      } else if(x == 0){
        edgenum = 2;
      } else if(x == BOARD_WIDTH-1){
        edgenum = 3;
      } else if(y == 0){
        edgenum = 1;
      } else if(y == BOARD_WIDTH-1){
        edgenum = 0;
      }
      for(int pIndex = 0; pIndex < EDGE_TOT; pIndex++){
        bool patternMatched = true;
        for(int c = 0; c < 2; c++){
          for(int coord = 0 ; coord < EDGE_MAX_COORDS; coord++){
            if(edge_pattern[edgenum][pIndex][c][coord][0] != X){
              int xCheck = x + edge_pattern[edgenum][pIndex][c][coord][0];
              int yCheck = y + edge_pattern[edgenum][pIndex][c][coord][1];
              Tile res = GetBoard(xCheck, yCheck);
              if(c == 0){
                if(res != colourA) patternMatched = false;
              } else {
                if(res != colourB) patternMatched = false;
              }
            }
          }
        }
        if(patternMatched){
          return make_tuple(true, pIndex);
        }
      }


      return make_tuple(false, 0);
    }

    tuple<bool, int> CheckForDeadCell(int x, int y){ // Obviously dont run this on dead cells
      if(x != 0 && x != BOARD_WIDTH-1 && y != 0 && y != BOARD_WIDTH-1){
        int colourSel1 = 0;
        int colourSel2 = 1;
        for(int colourNum = 0; colourNum < 2; colourNum++){
          for(int patternIndex = 0; patternIndex < PATTERN_TOT; patternIndex++){

            bool patternMatched = true;
            for(int redCoord = 0; redCoord < PATTERN_MAX_COORDS; redCoord++){
              if(patterns[patternIndex][colourSel1][redCoord][0] != X){
                int xToCheck = x + patterns[patternIndex][colourSel1][redCoord][0];
                int yToCheck = y + patterns[patternIndex][colourSel1][redCoord][1];
                if(!(xToCheck < 0 || yToCheck < 0 || xToCheck >= BOARD_WIDTH || yToCheck >= BOARD_WIDTH)){
                  Tile res = GetBoard(xToCheck, yToCheck);
                  if(res != PLAYER_RED){
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
                  if(!(xToCheck < 0 || yToCheck < 0 || xToCheck >= BOARD_WIDTH || yToCheck >= BOARD_WIDTH)){
                    Tile res = GetBoard(xToCheck, yToCheck);
                    if(res != PLAYER_BLUE){
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
              return make_tuple(true, patternIndex);
            }

          }
          colourSel1 = 1;
          colourSel2 = 0;
        }
        return make_tuple(false, 0);
      } else {
        return CheckCorner(x,y);
      }



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
    void PrintPatterns(){
      // Purely for testing purposes
      int x = 6;
      int y = 6;
      for(int p = 0; p < PATTERN_TOT; p++){
        Board b(false);
        for(int c = 0; c < 2; c++){
          for(int coords = 0; coords < PATTERN_MAX_COORDS; coords++){
            if(c == 0){
              b.SetBoard(x + patterns[p][c][coords][0], y + patterns[p][c][coords][1], PLAYER_RED);
            } else {
              b.SetBoard(x + patterns[p][c][coords][0], y + patterns[p][c][coords][1], PLAYER_BLUE);
            }


          }
        }
        b.PrintBoard();
      }
    };
    void PrintEdgePatterns(){
      // Purely for testing purposes
      int x,y;
      for (int edge = 0; edge < 4; edge++){
        if(edge == 0){ //bottom
          x = 6; y = BOARD_WIDTH-1;
        } else if(edge == 1){ //top
          x = 6; y = 0;
        } else if(edge == 2){ //left
          x = 0; y = 6;
        } else { //right
          x = BOARD_WIDTH-1, y = 6;
        }
        for (int p = 0; p < EDGE_TOT; p++){
          cout << "Edge: " << edge << " Pattern: " << p << endl;
          Board b(false);
          for(int c = 0; c < 2; c++){
            for(int coords = 0; coords < EDGE_MAX_COORDS; coords++){
              if(edge_pattern[edge][p][c][coords][0] != X){
                int xCheck = x + edge_pattern[edge][p][c][coords][0];
                int yCheck = y + edge_pattern[edge][p][c][coords][1];
                cout << "(" << xCheck << ", " << yCheck << ")\n";
                if(c == 0){
                  b.SetBoard(xCheck, yCheck, PLAYER_RED);
                } else {
                  b.SetBoard(xCheck, yCheck, PLAYER_BLUE);
                }
              }

            }
          }
          b.PrintBoard();
        }
      }
    }

};


int main() {
  Board b(true);
  int x = 2;
  int y = 2;
  b.PrintEdgePatterns();
  auto t1 = high_resolution_clock::now();
  /*
  for(int y = 0; y < BOARD_WIDTH; y++){
   for(int x = 0; x < BOARD_WIDTH; x++){
     if(b.GetBoard(x,y) == Board::EMPTY_COLOUR){
       tuple<bool, int> result = b.CheckForDeadCell(x,y);
       if(get<0>(result)){
         cout << "Dead Cell found at: (" << x << ", " << y << ") Rule used: " << get<1>(result) << endl;
       }
     }
   }
 }*/

  //tuple<bool, int> result = b.CheckForDeadCell(12, 10);
  //cout << result.get<0>;
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  cout << "Time taken: " << ms_double.count() << "ms" <<endl;

  return 0;
};
