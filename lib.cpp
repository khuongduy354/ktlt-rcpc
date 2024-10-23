#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#elif __linux
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif
using namespace std;
class Socket
{
private:
    int socketDescriptor;

public:
    Socket() { socketDescriptor = socket(AF_INET, SOCK_STREAM, 0); }
    // code for server to host
    void hostServer(int port = 8080)
    {
        // specifying the address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // binding socket.
        bind(socketDescriptor, (struct sockaddr *)&serverAddress,
             sizeof(serverAddress));
    };

    void listenForConnection(void (*cbFuncPointer)(char[1024]))
    {
        // listening to the assigned socket
        listen(socketDescriptor, 5);

        // accepting connection request
        while (int clientSocket = accept(socketDescriptor, nullptr, nullptr))
        {
            // recieving data
            char buffer[1024] = {0};
            while (1)
            {
                memset(&buffer, 0, sizeof(buffer));
                int nread = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (nread == 0)
                    break;
                if (nread == -1)
                {
#ifdef _WIN32
                    cout << "Recv failed: " << WSAGetLastError() << endl;
#elif unix
                    cout << "Recv failed" << endl;
#endif
                    break;
                }
                cbFuncPointer(buffer);
            }
        }
    };

    // code for client to connect to server
    void connectServer(string serverIP, int port = 8080)
    {
        // specifying address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());

        // sending connection request
        connect(socketDescriptor, (struct sockaddr *)&serverAddress,
                sizeof(serverAddress));
    };
    void sendMessage(string msg)
    {
        send(socketDescriptor, msg.c_str(), msg.size(), 0);
    };
    void closeConnection() { close(socketDescriptor); }
};