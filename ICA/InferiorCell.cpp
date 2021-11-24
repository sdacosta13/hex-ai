#include <iostream>
using namespace std;


#define BOARD_WIDTH 5
#define USE_COLOR true
// Unsure of what the states are represented as
#define PLAYER_1_COLOUR 1
#define PLAYER_2_COLOUR -1


class Board{
  public:
    int board [BOARD_WIDTH][BOARD_WIDTH] = {};
    void SetBoard(int x, int y, int value){
      board[y][x] = value;
    }
    int GetBoard(int x, int y){
      return board[y][x];
    }
    void PrintBoard(){
      for(int y = 0; y < BOARD_WIDTH; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
          if(GetBoard(x, y) == PLAYER_1_COLOUR){
            if(USE_COLOR){
              cout << "\e[1;31mR\e[0m";
            } else {
              cout << 'R';
            }

          } else if (GetBoard(x, y) == PLAYER_2_COLOUR) {
            if(USE_COLOR){
              cout << "\e[1;34mB\e[0m";
            } else {
              cout << 'B';
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
    Board(){
      for(int y = 0; y < BOARD_WIDTH; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
          int r = rand() % 2; // Just for testing purposes fill the board with random positions
          if(r == 0){
            SetBoard(x, y, PLAYER_1_COLOUR);
          } else {
            SetBoard(x, y, PLAYER_2_COLOUR);
          }
        }
      }
    }

};


int main() {
  Board b;
  b.PrintBoard();
  return 0;
};
