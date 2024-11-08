using namespace std;
#include "lib.cpp"

void receiveDataFromServer(SOCKET s) 
{
    char buffer[BUFFER_SIZE];

    char fileType[BUFFER_SIZE];
    recv(s, fileType, BUFFER_SIZE, 0);
    string fileName = "received_file" + (string)fileType;
    ofstream outputFile(fileName, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Failed to open file for writing: " << fileName << endl;
        return;
    }

    cout << "Receiving file from server and saving as " << fileName << "..." << endl;

    int bytesReceived;
    while ((bytesReceived = recv(s, buffer, BUFFER_SIZE, 0)) > 0) {
        outputFile.write(buffer, bytesReceived);
    }

    if (bytesReceived == SOCKET_ERROR) {
        cout << "recv() failed: " << WSAGetLastError() << endl;
    } 
    else if (bytesReceived == 0) {
        cout << "File received successfully. Connection closed by server." << endl;
    }

    outputFile.close();
}

int main()
{
#ifdef _WIN32
  WSADATA wsa;
  // configuration

  // Khởi tạo winsock
  std::cout << "Initializing Winsock..." << std::endl;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
    return 1;
  }
#endif
  Socket socket;
  socket.connectServer("127.0.0.1");
  socket.sendMessage("Hello");
  socket.sendMessage("captureScreen-filepath: ");
  while (1)
    ;
}
