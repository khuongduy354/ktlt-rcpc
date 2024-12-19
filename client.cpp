#include "lib.cpp"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

void receiveDataFromServer(SOCKET s) 
{
    char buffer[BUFFER_SIZE];

    string fileType = "";
    int typeLength;
    if (recv(s, (char*)&typeLength, sizeof(typeLength), 0) == SOCKET_ERROR)
      cout << "Cannot recv type length" << endl;
    fileType.resize(typeLength);
    recv(s, &fileType[0], typeLength, 0);
      string fileName = "received_file" + fileType;
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

  // initialize socket
  Socket socket;
  cout << "Connecting to server..." << endl;
  int res = socket.connectServer("127.0.0.1");
  if (res < 0)
  {
    cout << "Failed to connect to server" << endl;
    return -1;
  }
  cout << "Connected to server" << endl;

  // Capture screen
  socket.sendMessage("captureScreen-filepath: ");

  // initialize mail API
  string access_token;
  cout << "Please input access_token:" << endl;
  cin >> access_token;

  int currMailIdx = 0;
  GmailAPI g(access_token);

  while (1)
  {
    cout << "Reading all mails..." << endl;
    string mailStr = g.readMail();

    if (mailStr == "error")
      return -1;

    // check if there is new mail
    json mailJson = json::parse(mailStr);
    cout << "Checking new mail..." << endl;
    if (currMailIdx >= mailJson["value"].size())
    {
      cout << "No new mail to processed " << endl;
      sleep(5);
      continue;
    }
    else
      cout << "New mail available, processing new mail..." << endl;

    // extract mail content to to use as command
    string body = mailJson["value"][currMailIdx]["body"]["content"];
    string subject = mailJson["value"][currMailIdx]["subject"];
    string command = subject + ":" + body;

    // request to server
    cout << "Sending command: " << command << " to server, waiting for server response..." << endl;
    string res = socket.sendMessage(command);
    cout << "Server response: " << res << endl;

    // if server response, sendMail back to notify user
    if (res == "ok")
    {
      cout << "Server response success, sending mail back to Outlook..." << endl;
      string sendMailRes = g.sendMail(command + "executed");
      if (sendMailRes == "error")
      {
        cout << "Send mail failed" << endl;
        break;
      }
      else
      {
        cout << "Send mail success" << endl;
        currMailIdx++;
      }
    }
    else
      cout << "Server response failed" << endl;

    sleep(10);
  }
}
