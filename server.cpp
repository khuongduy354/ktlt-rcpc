#include "lib.cpp"
#include <gdiplus.h>

// Support both Linux and Windows machine
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#elif unix
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

using namespace std;
using namespace Gdiplus;

void serverCallback(char buffer[1024]){
    cout << "Client: " << buffer << endl;
    
}

void shutDownComputer(){
    cout << "Computer will shut down in 30 second!";
    system("C:\\windows\\system32\\shutdown /s /t 30 \n\n");
}   

void restartComputer(){
    cout << "Computer will restart in 30 second!";
    system("C:\\windows\\system32\\shutdown /r /t 30\n\n");
}

bool sendDataToClient(SOCKET s, const string &filePath) 
{
    char buffer[BUFFER_SIZE];

    int f = filePath.find('.');
    string fileType = filePath.substr(f);
    send(s, (char *)&fileType, fileType.length() + 1, 0);

    ifstream file(filePath, ios::binary);
    if (!file.is_open()) 
    {
        cerr << "Could not open file: " << filePath << endl;
        return false;
    }

    cout << "Sending file: " << filePath << " to client." << endl;

    while (file.read(buffer, BUFFER_SIZE) || file.gcount() > 0) 
    {
        int bytesToSend = static_cast<int>(file.gcount());
        if (send(s, buffer, bytesToSend, 0) == SOCKET_ERROR) 
        {
            cerr << "Failed to send data to client." << endl;
            file.close();
            return false;
        }
    }
    file.close();
    cout << "File sent successfully." << endl;
    return true;
}

void sendListApps(SOCKET s) {
    system("wmic product get name,version > listApps.txt");
    Sleep(20000);
    sendDataToClient(s, "listApps.txt");
} 

void startApp(const string &appName) {
    string s = "start " + appName;
    system(s.c_str());
}

void stopApp(const string &appName) {
    string s = "taskkill /IM " + appName + ".exe /F";
    system(s.c_str());
}

void sendListServices(SOCKET s) {
    system("wmic service get name, displayname, state > listServices.txt");
    Sleep(20000);
    sendDataToClient(s, "listApps.txt");
}

void startService(const string &serviceName) {
    string s = "sc start " + serviceName;
    system(s.c_str());
}

void stopService(const string &serviceName) {
    string s = "sc stop " + serviceName;
    system(s.c_str());
}

void deleteFile(const string &filePath) {
    string s = "del " + filePath;
    system(s.c_str());
}

// function that handle png/jpec image
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid){
    UINT num = 0;   // number of image encoders
    UINT size = 0;  // size of the image encoder array in bytes

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;

    ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)malloc(size);
    if(pImageCodecInfo == NULL)
        return -1;

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; j++){
        if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0){
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;
        }
    }

    free(pImageCodecInfo);
    return -1;
}


// CaptureScreen
void captureScreen(const string &file_path){
    //HDC stand for handle to device context it use to obtain graphical component
    HDC hdcScreen = GetDC(NULL); 
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
    int width = GetSystemMetrics(SM_CXSCREEN); // get the width of the Screen
    int height = GetSystemMetrics(SM_CYSCREEN); // get the height of the Screen
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height); // this is a handle to GDI
    //Initialize gdi+
    GdiplusStartupInput gdip;
    ULONG_PTR gdipToken;
    GdiplusStartup(&gdipToken, &gdip, NULL);
    //capture screen
    HGDIOBJ holdBitmap = SelectObject(hdcMemDC, hbmScreen);
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    CLSID encoderID;
    GetEncoderClsid(L"image/png", &encoderID);
    Bitmap bmp(hbmScreen, NULL);
    std::wstring wide_path(file_path.begin(), file_path.end());
    bmp.Save(wide_path.c_str(), &encoderID, NULL);
    //clean up
    SelectObject(hdcMemDC, holdBitmap);
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    GdiplusShutdown(gdipToken);
}

int main() {
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
    
    captureScreen("E:\\ProjectMMT\\ktlt-rcpc\\screenshot.png");
    return 0;
}
