#include "lib.cpp"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

void writeToFile(string buffer, string filename)
{
  ofstream outputFile(filename, ios::binary);
  outputFile.write(buffer.c_str(), buffer.size());
  outputFile.close();
}
string receiveDataFromServer(SOCKET s, string filename)
{
  char buffer[BUFFER_SIZE];
  int bytesReceived;
  string total;
  while ((bytesReceived = recv(s, buffer, BUFFER_SIZE, 0)) > 0)
  {
    total.append(buffer, bytesReceived);
  }

  if (bytesReceived == SOCKET_ERROR)
  {
    cout << "recv() failed: " << WSAGetLastError() << endl;
  }
  else if (bytesReceived == 0)
  {
    cout << "File received successfully. Connection closed by server." << endl;
  }

  return total;
}
string encodeBase64(string input)
{
  static const char base64_chars[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";

  string encoded;
  size_t i = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  for (size_t index = 0; index < input.size(); index++)
  {
    char_array_3[i++] = input[index];
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; i < 4; i++)
        encoded += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for (size_t j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (size_t j = 0; j < i + 1; j++)
      encoded += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      encoded += '=';
  }
  return encoded;
}

string readFile(string filePath)
{
  ifstream file(filePath, ios::binary);

  char buffer[1024];
  string total;
  while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0)
  {
    total.append(buffer, file.gcount());
  }
  total.append(buffer, file.gcount());

  file.close();
  return total;
}
int mailMain(Socket socket, GmailAPI g);
int main()
{
#ifdef _WIN32
  // Set console code page to UTF-8 so console known how to interpret string data
  SetConsoleOutputCP(CP_UTF8);

  // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
  setvbuf(stdout, nullptr, _IOFBF, 1000);

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

  // initialize mail API
  string access_token;
  cout << "Please input access_token:" << endl;
  cin >> access_token;

  GmailAPI g(access_token);

  mailMain(socket, g);
}

int mailMain(Socket socket, GmailAPI g)
{

  int currMailIdx = 0;

  while (1)
  {

    cout << "Reading all mails..." << endl;
    string mailStr = g.readMail();
    json mailJson = json::parse(mailStr);

    while (1)
    {
     socket.reconnectServer("127.0.0.1");
      // process all mails in this batch
      // check new mail 
      cout << "Checking new mail..." << endl;
      if (currMailIdx >= mailJson["value"].size())
      {
        cout << "No new mail to processed " << endl;
        sleep(10);
        break;
      }
      else
        cout << "New mail available, processing new mail..." << endl;

      // extract mail content to to use as command
      auto body = mailJson["value"][currMailIdx]["bodyPreview"];
      cout << "body: " << mailJson.dump(4) << endl;
      if (body.is_null())
        body = "";
      string subject = mailJson["value"][currMailIdx]["subject"];
      string payload = subject + " " + (string)body;
      string command = subject;

      // request to server
      cout << "Sending payload: " << payload << " to server, waiting for server response..." << endl;
      string res = socket.sendMessage(payload);
      cout << "Server responded." << endl;
      vector<string> hasAttach = {"getfile", "recordwebcam", "keylogger", "screenshot", "listapp", "listservice"};
   
      // if server response, sendMail back to notify user
      auto it = find(hasAttach.begin(), hasAttach.end(), command);
      if (it == hasAttach.end())
      {
        cout << "Sending no attachment mail " << endl;
        g.sendMail(command, "ok");
      }
      else
      {
        cout << "Sending mail with attachment: " << res << endl;
        // string extension = "";
        // extension = command == "recordwebcam" ? ".avi":
        //             command == "keylogger" ? ".txt":
        //             command == "screenshot" ? ".png" : ".txt" ;
        string attachName = body;
        if (command == "recordwebcam")
          attachName = "video.avi";
        if (command == "screenshot")
          attachName = "screenshot.png";
        if (command == "listapp" || command == "listservice" || command == "keylogger")
          attachName = "list.txt";
        g.sendMail(command, " ", attachName, encodeBase64(res));
        sleep(1);
      }

      currMailIdx++;
      cout << "Sent mail successfully" << endl;
      socket.closeConnection();
      sleep(10);
    }
  }
}
