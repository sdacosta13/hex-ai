#include <stdio.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
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

bool interpretMessage(vector<string> messageFromServer){
  string messageCategory = messageFromServer.front();
  if (messageCategory.compare("START") == 0){
    //TODO: intialize the board and game on our end, and then make the first move

    //if we get start, then make a move to start the game (placeholder for demonstration)
    //this should be within makeMove function anyway
    string moveToSend = "2,4\n";
    sendMessage(moveToSend);
  }
  else if(messageCategory.compare("CHANGE") == 0){
    if(messageFromServer[3].compare("END") == 0){ return false; }
    else if(messageFromServer[1].compare("SWAP") == 0){
      //TODO: if the opponent has swapped we are a different color, and should update that
    }
    else if(messageFromServer[3].compare("*Our Color*")){
      //TODO: if its our turn, then make a move.
    }
    else{
      //TODO: something went wrong (probaly remove this else statement)
    }
  }
  else{
    return false;
  }
  return true;
}

int makeMove(string opponentsMove){
  return 0;
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
