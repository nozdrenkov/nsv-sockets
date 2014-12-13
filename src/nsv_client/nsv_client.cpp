#include <iostream>
#include <string>
#include <winsock2.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
const unsigned short port = 5001;
const char ip[] = "127.0.0.1";

void dbg(const string &s) { cout << s << endl; }
void die(const string &s) { dbg(s); exit(0); }

int main()
{
    dbg("TCP Client");
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
    sai.sin_addr.s_addr = inet_addr(ip);

    dbg("Connecting...");
    if (connect(s, (sockaddr*)(&sai), sizeof (sai)) == SOCKET_ERROR)
        die("Connect error!");
    dbg("Data exchanging...");

    string msg = "From client";
    
    dbg("Sending message...");
    send(s, msg.c_str(), msg.size(), 0);

    while (true)
    {
        char recvchar;
        int received = recv(s, &recvchar, 1, 0);
        if (received == SOCKET_ERROR)
            die("Socket Error: receive");

        if (received)
            printf("%c", recvchar);
        else
        {
            dbg("Connection closed by remote host");
            break;
        }
    }
    closesocket(s);
    return 0;
}