// C++ program to illustrate the client application in the
// socket programming
// #include <arpa/inet.h>
#include <cstring>
#include <iostream>
// #include <netinet/in.h>
#include <string>
// #include <sys/socket.h>
#include <unistd.h>
#include <winsock2.h>

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
  void sendMessage(string msg) { send(clientSocket, msg.c_str(), strlen(msg.c_str()), 0); };
  void closeConnection() { close(clientSocket); }
};

int main() {
      WSADATA wsa;
    // configuration

    //Khởi tạo winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }
  Socket socket;
  socket.connectServer("127.0.0.1");
  socket.sendMessage("Hello");
  while(1);
}
