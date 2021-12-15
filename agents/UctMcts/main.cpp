#include "GameState.h"
#include "UctMcts.h"

#include <stdio.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <iostream>
using std::vector;
using std::string;

#define PORT 1234
#define HOST "127.0.0.1"

// Variable to store the socket
int serverSocket;

// Variable to store the server address
struct sockaddr_in serverAddress;

//Variable to store messages from the server
char serverMessageBuffer[1024] = {0};

//Variable to
bool messageResolved;
vector<string> messageFromServer;

string ourColor;
int turn = 0;

GameState gameState = GameState();
UctMcts tree = UctMcts(gameState);

int connectToServer(){
  // if the return from the server socket call fails abort
  if( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("\n Socket Creation Failed \n");
    return -1;
  }

  //define the address for socket connection
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  //connect and abort if this fails
  if(connect(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
    printf("\n Connection Failed \n");
    return -1;
  }
  return 0;
}

int closeSocket(){
  //Close the socket to disconnect
  int close(serverSocket);
  return 0;
}

vector<string> getMessage(){
  //read the message, and store to the buffer

  int messageResult = read(serverSocket, serverMessageBuffer, 1024);

  //loop through buffer and get the message, storing to list

  //trim the string, as with example agents
  string messageOutOfBuffer = string(serverMessageBuffer);
  size_t first = messageOutOfBuffer.find_first_not_of(' ');
  size_t last = messageOutOfBuffer.find_last_not_of(' ');
  messageOutOfBuffer = messageOutOfBuffer.substr(first, (last-first+1));

  //implementation of "split" function from other languages
  vector<string> messageToReturn;
  string splitToken;
  // delimiter for message is ";"
  std::stringstream strStream(messageOutOfBuffer);
  while(getline(strStream, splitToken, ';')){
    messageToReturn.push_back(splitToken);
  }

  //clear the buffer as we have read the messages from it
  //commented out, as assuming the next "read" will overwrite the buffer ?? seems to be the case
  //serverMessageBuffer[] = {0};

  return messageToReturn;
}

int sendMessage(string messageToSend){
  send(serverSocket, &messageToSend[0], messageToSend.size(), 0);
  return 0;
}

void makeMove(std::string moveString, std::tuple<int, int> moveTuple){
    sendMessage(moveString);
    gameState.play(moveTuple);
};

bool interpretMessage(vector<string> messageFromServer){
  turn++;
  string messageCategory = messageFromServer.front();
  if (messageCategory.compare("START") == 0){
    ourColor = messageFromServer[2];
    if(ourColor.compare("R") == 0){
      Node rootNode = Node();
      rootNode.coord = std::make_tuple(9, 7);
      tree.rootNode = rootNode;
      string moveToSend = "9,7\n";
      makeMove(moveToSend, std::make_tuple(9, 7));
      return true;
    }
  }
  else if(messageCategory.compare("CHANGE") == 0){
    if(messageFromServer[3].compare("END") == 0){ return false; }
    else if(messageFromServer[1].compare("SWAP") == 0){
      //TODO: if the opponent has swapped we are a different color, and should update that
    }
    else {
        std::stringstream opponentMove(messageFromServer[1]);
        vector<int> opponentMoveCoords;

        while(opponentMove.good()){
            string temp;
            getline( opponentMove, temp, ',' );
            opponentMoveCoords.push_back(atoi(temp.c_str()));
        }
        //get x and y of opponents move
        int opponentMoveX = opponentMoveCoords[0];
        int opponentMoveY = opponentMoveCoords[1];

        //if we are on the turn to be given the swap option
        if (turn == 2 && opponentMoveX!=2){ //to add, and intepreted move matches swap map
            sendMessage("SWAP\n");
            return true;
        }
        // nMoves =  min( numberOfMovesOutOfBook, 10 );
        // factor = 2 -  nMoves / 10 
        // target = timeLeft / numberOfMovesUntilNextTimeControl
        // time   = factor * target
        float time = 0.2;
        tree.search(time);
        std::tuple<int, int> bestMove = tree.getBestMove();
        std::string bestMoveString = std::to_string(std::get<0>(bestMove)) + "," + std::to_string(std::get<1>(bestMove)) + "\n";
        std::cout << "hello";
        makeMove(bestMoveString, bestMove);
    };
  }
  else{
    return false;
  }
  return true;
}

int runAgent(){
  //connect the agent
  if(connectToServer() < 0){
    printf( "\n Issue with server connection, aborting \n");
    closeSocket();
    exit(0);
  }
  //run the agent by getting messages from the server, then doing something with these messages
  do{
    messageFromServer = getMessage();
    messageResolved = interpretMessage(messageFromServer);
  }while(messageResolved); // if message ever returns false after interpret, then end the agent

  closeSocket();
  return 0;
}

int main(int argc , char *argv[]){
  //run the agent
  int returnCode = runAgent();
  //print after we have finished the agent, and return
  printf("\n Finished Running cpp Agent with code %d \n", returnCode);
  return 0;

}