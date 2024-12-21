#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
using json = nlohmann::json;

#include <fstream>
#ifdef _WIN32
#include <winsock2.h>
#elif __linux
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
using namespace std; 

//helper  

 
// socket
const int BUFFER_SIZE = 1024;
class Socket
{
private:
    int socketDescriptor;

public:

    int getSocketDescriptor(){
        return socketDescriptor;
    }
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

    void listenForConnection(void (*cbFuncPointer)(char[1024], int clientSocket))
    {
        // listening to the assigned socket
        listen(socketDescriptor, 5);

        // accepting connection request
        while (int clientSocket = accept(socketDescriptor, nullptr, nullptr))
        { 
            cout << "Client connected" << endl;
            // recieving data
            char buffer[1024] = {0};
            while (1)
            {
                memset(&buffer, 0, sizeof(buffer));
            cout << "before  " << endl;
                int nread = recv(clientSocket, buffer, sizeof(buffer), 0);
            cout << "after" << endl;
                if (nread == 0){
                    cout << "No bytes received, terminating client" << endl;
                    break;
                }
                if (nread == -1)
                {
#ifdef _WIN32
                    cout << "Recv failed: " << WSAGetLastError() << endl;
#elif unix
                    cout << "Recv failed" << endl;
#endif
                    break;
                }
            cbFuncPointer(buffer, clientSocket);
            shutdown(clientSocket, SD_SEND);
            }
            cout << "Client disconnected" << endl;
        }
    };

 // code for client to connect to server
    int reconnectServer(string serverIP, int port = 8080)
    {
        socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        return connectServer(serverIP, port); 
    };
    // code for client to connect to server
    int connectServer(string serverIP, int port = 8080)
    {
        // specifying address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());

        // sending connection request
        int res =  connect(socketDescriptor, (struct sockaddr *)&serverAddress,
                       sizeof(serverAddress));
        if (res < 0)
        {
            cout << "Failed to connect to server" << endl;
            return -1;
        }
        cout << "Connected to server" << endl;      

        return res;
    };
    string sendMessage(string msg)
    {
        send(socketDescriptor, msg.c_str(), msg.size(), 0);

        // receive
        char buffer[1024];  
        memset(&buffer, 0, sizeof(buffer)); // clear the buffer
        string total;
        while (1)
        {
            int bytesRead = recv(socketDescriptor, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0)
                break; 

            total.append(buffer, bytesRead);
        }
        return total;
    };
    void closeConnection() { 
#ifdef _WIN32   
        closesocket(socketDescriptor);
#elif unix
        close(socketDescriptor); 
#endif
        }
};

class GmailAPI
{
private:
    enum HTTPMethod
    {
        GET = 1,
        POST = 2
    };
    string jwt;
    const string baseUrl = "https://graph.microsoft.com/v1.0";
    string ARCHIEVED_ID = "AQMkADAwATM3ZmYAZS0wZTQxLWMyZjEtMDACLTAwCgAuAAADDb71hPR08EuaBlxUX-mb1QEAU0nGv_8xP0uYqHthDiOW8AAAAgFMAAAA";
    string latestTimeStamp = "";

public:
    GmailAPI(string _jwt)
    {
        jwt = _jwt;
    }

    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
    {
        ((string *)userp)->append((char *)contents, size * nmemb);
        return size * nmemb;
    };
    string fetch(string url, HTTPMethod method, vector<string> headers = {}, string payload = "")
    {
        CURL *curl_handle = curl_easy_init();
        if (curl_handle)
        {
            // prep headers
            curl_slist *headersL = NULL;
            string authHeader = "Authorization: Bearer " + jwt;
            headersL = curl_slist_append(headersL, authHeader.c_str());
            for (string header : headers)
            {
                headersL = curl_slist_append(headersL, header.c_str());
            }
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headersL);

            // prep payload & method
            if (method == HTTPMethod::POST)
            {
                curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, payload.c_str());
            }

            // prep request
            string readBuffer = "";
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &readBuffer);

            // Bypass SSL verification (NOT SECURE)
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

            // execute request
            CURLcode res = curl_easy_perform(curl_handle);

            if (res != CURLE_OK)
            {
                fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
                curl_slist_free_all(headersL); /* free the list */
                cout << "Curl error: " <<  res << endl;
                cout << "Curl error buffer: " <<  readBuffer << endl;
                return "error";
            }
            else
            {
                curl_slist_free_all(headersL); /* free the list */
                return readBuffer;
            }
        }
    }
    wstring fetchw(string url, HTTPMethod method, vector<string> headers = {}, string payload = "")
    {
        CURL *curl_handle = curl_easy_init();
        if (curl_handle)
        {
            // prep headers
            curl_slist *headersL = NULL;
            string authHeader = "Authorization: Bearer " + jwt;
            headersL = curl_slist_append(headersL, authHeader.c_str());
            for (string header : headers)
            {
                headersL = curl_slist_append(headersL, header.c_str());
            }
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headersL);

            // prep payload & method
            if (method == HTTPMethod::POST)
            {
                curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, payload.c_str());
            }

            // prep request
            wstring readBuffer;
            curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &readBuffer);

            // Bypass SSL verification (NOT SECURE)
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);

            // execute request
            CURLcode res = curl_easy_perform(curl_handle);

            if (res != CURLE_OK)
            {
                fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
                curl_slist_free_all(headersL); /* free the list */
                // cout << "Curl error: " <<  res << endl;
                // cout << "Curl error buffer: " <<  readBuffer << endl;
                return readBuffer;
            }
            else
            {
                curl_slist_free_all(headersL); /* free the list */
                return readBuffer;
            }
        }
    }
    string readMail()
    {
        // read all mails from Archieve folder
        // string url = baseUrl + (string) "/me/mailFolders/" + ARCHIEVED_ID + "/messages";
        
        string selectQuery = "?$select=body,subject,bodyPreview&$filter=(from/emailAddress/address)\%20eq\%20'23127478@student.hcmus.edu.vn'";
        string url = baseUrl + (string) "/me/messages" + selectQuery;
        string res = fetch(url, HTTPMethod::GET, {"Prefer: outlook.body-content-type=\"text\""});
        return res;
    }

    string sendMail(string subject, string content = "", string attachmentName = "", string attachmentBytes = "")
    { 
        // create draft
        json mailPayload = {
            {"subject", subject},
            {"importance", "Low"},
            {"body", {{"contentType", "HTML"}, {"content", content}}},
            {"toRecipients", {{{"emailAddress", {{"address", "23127478@student.hcmus.edu.vn"}}}}}}};


        string draftUrl = baseUrl + "/me/messages";
        string contentLengthHeader =
            "Content-Length: " +
            to_string(mailPayload.dump().size());

        // create draft
        string draftStr = fetch(draftUrl, HTTPMethod::POST, {"Content-Type: application/json", contentLengthHeader}, mailPayload.dump());

        if (draftStr == "error")
            return "error";

        // extract draft id
        json draftJs = json::parse(draftStr);
        string id = draftJs["id"];

        // add attachment if any 
        if(attachmentName != "" && attachmentBytes !=""){  
            string attachmentUrl = "https://graph.microsoft.com/v1.0/me/messages/"+id+"/attachments"; 
            json attachmentPayload = {
            {"@odata.type", "#microsoft.graph.fileAttachment"},
            { "name", attachmentName},
            {"contentBytes", attachmentBytes},
            }; 

        string contentLengthHeader =
            "Content-Length: " +
            to_string(attachmentPayload.dump().size());

            string atRes = fetch(attachmentUrl,  HTTPMethod::POST, {"Content-Type: application/json", contentLengthHeader},attachmentPayload.dump());
            cout << atRes << endl;

        }

        // send mail https://graph.microsoft.com/v1.0/me/messages/{id}/send
        
        // send mail
        // string sendUrl = baseUrl + "/me/messages/" + id + "/send";

        // string res = fetch(sendUrl, HTTPMethod::POST, {"Content-Length: 0"});
        // cout << "Send mail response: " << res << endl;
        return id;
    }

};