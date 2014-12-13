#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
const unsigned short port = 5001;

void dbg(const string &s) { cout << s << endl; }
void die(const string &s) { dbg(s); exit(0); }

int main()
{
    dbg("TCP Server");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData))
        die("Can't startup Windows Sockets");
    dbg("Windows Sockets started");
     
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
        die("Can't create socket");
    dbg("Socket Created");
    
    sockaddr_in sai;
    memset(&sai, 0, sizeof (sai));
    sai.sin_family = AF_INET;
    sai.sin_port = htons(port);
    sai.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(s, (sockaddr*)(&sai), sizeof(sai)) == SOCKET_ERROR)
        die("Bind error!");
    dbg("Bind OK!");
    
    if (listen(s, 1) == SOCKET_ERROR)
        die("Listen error");
    dbg("Listen OK!");

    while (true)
    { 
        dbg("Accepting...");
        struct sockaddr_in saic;
        int addrlen;
        SOCKET sc = accept(s, (sockaddr *) &saic, &addrlen);
        if (sc == INVALID_SOCKET)
        {
            dbg("Accept error!");
            break;
        }
        
        dbg("Connection opened:");
        dbg(inet_ntoa(saic.sin_addr));
        dbg("Data exchanging...");
        
        string msg = "From Server!";
        dbg("Sending msg...");
        send(sc, msg.c_str(), msg.size(), 0);
        
        while (true)
        {
            char recvchar;
            int received = recv(sc, &recvchar, 1, 0);
            if (received == SOCKET_ERROR)
                die("Socket Error: receive");
            
            if (received)
                printf("%c", recvchar);
            else
            {
                dbg("Connection closed by remote host!");
                break;
            }
        }
        closesocket(sc);
    }
    closesocket(s);
    return 0;
}