// C++ program to illustrate the client application in the
// socket programming
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class Socket {
private:
  int clientSocket;

public:
  Socket() { clientSocket = socket(AF_INET, SOCK_STREAM, 0); }
  void hostServer(){};

  void connectServer(string serverIP, int port = 8080) {

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());

    // sending connection request
    connect(clientSocket, (struct sockaddr *)&serverAddress,
            sizeof(serverAddress));
  };
  void sendMessage(string msg) {
    send(clientSocket, msg.c_str(), msg.size(), 0);
  };
  void closeConnection() { close(clientSocket); }
};

int main() {}
