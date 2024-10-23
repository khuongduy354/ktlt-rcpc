// C++ program to show the example of server application in
// socket programming
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
// #include <netinet/in.h>
// #include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    WSADATA wsa;
    // configuration
    int PORT = 8080;

    //Khởi tạo winsock
    std::cout << "Initializing Winsock..." << std::endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr *)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    while (int clientSocket = accept(serverSocket, nullptr, nullptr))
    {
        // recieving data
        char buffer[1024] = {0};
        while (1)
        {
            memset(&buffer, 0, sizeof(buffer));
            int nread = recv(clientSocket, buffer, sizeof(buffer), 0);
            cout << nread << endl; 
            if (nread == 0)
                break;
        if (nread == SOCKET_ERROR) {
            std::cout << "Recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
            cout << "Message from client: " << buffer
                 << endl;
         
        }
           close(serverSocket);
    }

    // closing the socket.
    close(serverSocket);

    return 0;
}
