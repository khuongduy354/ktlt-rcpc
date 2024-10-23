using namespace std;
#include "lib.cpp"

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
  while (1)
    ;
}
