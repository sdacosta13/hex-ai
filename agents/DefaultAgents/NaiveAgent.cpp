#include <stdio.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

#define PORT 1234
#define HOST "127.0.0.1"


int serverSocket;
struct sockaddr_in serverAddress;

char serverMessageBuffer[1024] = {0};

bool messageResolved;
std::string messageFromServer;

int connectToServer(){

  if( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("\n Socket Creation Failed \n");
    return -1;
  }

  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);

  if(connect(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
    printf("\n Connection Failed \n");
    return -1;
  }
  return 0;
}

int closeSocket(){
  int close(serverSocket);
  return 0;
}

int getMessage(){
  int messageResult = read(serverSocket, serverMessageBuffer, 1024);
  return messageResult;
}

int sendMessage(std::string messageToSend){
  send(serverSocket, &messageToSend[0], messageToSend.size(), 0);
  return 0;
}

bool interpretMessage(std::string messageFromServer){
  // send a message that loses the game that is illegal
  sendMessage("SWApppP\n");
  return true;
}

int runAgent(){
  if(connectToServer() < 0){
    printf( "\n Issue with server connection, aborting \n");
    closeSocket();
    exit(0);
  }
  do{
    messageFromServer = getMessage();
    messageResolved = interpretMessage(messageFromServer);
  }while(messageResolved);

  closeSocket();
  return 0;
}

int main(int argc , char *argv[]){
  int returnCode = runAgent();
  printf("\n Finished Running cpp Agent with code %d \n", returnCode);
  return 0;
}
