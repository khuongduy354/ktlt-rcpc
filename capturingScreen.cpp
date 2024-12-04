// Support both Linux and Windows machine
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <conio.h>
#elif unix
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <cstring>
#include <iostream>
#include <string>
// #include "lib.cpp"
#include <unistd.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
#pragma comment(lib, "Gdiplus.lib")




using namespace std;
using namespace Gdiplus;

void serverCallback(char buffer[1024])
{
    cout << "Client: " << buffer << endl;
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
int main()
{
    captureScreen("E:\\ProjectMMT\\screenshot.png");
    return 0;
}
