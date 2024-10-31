#include <cstring>
#include <iostream>
#include <string>
#include "lib.cpp"
#include <unistd.h>

// Support both Linux and Windows machine
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#elif unix
#include <netinet/in.h>
#include <sys/socket.h>
#endif

using namespace std;

void serverCallback(char buffer[1024], int clientSocket)
{
    cout << "Client: " << buffer << endl;
    send(clientSocket, "ok", strlen("ok"), 0);
}
int main()
{
#ifdef _WIN32
    WSADATA wsa;

    // Khởi tạo winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }
#endif
    Socket socket;
    socket.hostServer();
    socket.listenForConnection(&serverCallback);

    return 0;
}
