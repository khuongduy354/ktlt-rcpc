#include "lib.cpp"
#include <iostream>
#include <string>
#include <c++json/single_include/nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

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
  int res = socket.connectServer("127.0.0.1");
  if (res < 0)
  {
    cout << "Failed to connect to server" << endl;
    return -1;
  }

  cout << "Connected to server" << endl;

  // initialize mail API
  string access_token;
  cout << "Please input access_token:" << endl;
  cin >> access_token;

  int currMailIdx = 0;
  GmailAPI g(access_token);
  while (1)
  {
    cout << "Checking new mail..." << endl;
    string mailStr = g.readMail();

    if (mailStr != "error")
    {
      json mailJson = json::parse(mailStr);

      // check new mail
      if (currMailIdx >= mailJson["value"].size())
      {
        for (int i = 0; i < mailJson["value"].size(); i++)
        {
          cout << "Mail " << i << ": " << mailJson["value"][i]["subject"] << endl;
        }
        cout << "No new mail to processed " << endl;
        sleep(5);
        continue;
      }
      else
      {
        cout << "Processing new mail..." << endl;
      }

      // extract subject + body
      string body = mailJson["value"][currMailIdx]["body"]["content"];
      string subject = mailJson["value"][currMailIdx]["subject"];

      // request to server
      string command = subject + ":" + body;
      cout << "Sending command: " << command << endl;
      string res = socket.sendMessage(command);

      // if server response, sendMail back
      if (res == "ok")
      {
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
    }

    sleep(5);
  }
}
