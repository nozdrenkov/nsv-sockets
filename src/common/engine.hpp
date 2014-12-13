#ifndef ENGINE_HPP
#define ENGINE_HPP
#pragma comment(lib, "WS2_32.lib")
#pragma comment(linker, "/STACK:36777216")

#include <iostream>
#include <string>
#include <cstring>
#include <WinSock2.h>
using namespace std;

#define die(s) { echo(s); return; }
#define dief(s) { echo(s); return false;}

class engine_t
{
    string type;

    WSADATA wsaData;
    SOCKET mysock, remsock;
    sockaddr_in sai;
    char buf[2000000];
public:
    void echo(const string &s) { cout << s << endl; }
    engine_t(const string &mtype, const string &ip, int port)
    {
        type = mtype;

        if (WSAStartup(MAKEWORD(2, 0), &wsaData))
            die("Can't startup Windows Sockets");
            echo("Windows Sockets started");

        if ((mysock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
            die("Can't create socket");
            echo("Socket Created");

        memset(&sai, 0, sizeof(sockaddr_in));
        sai.sin_family = AF_INET;
        sai.sin_port = htons(port);
        sai.sin_addr.s_addr = type == "server" ? INADDR_ANY : inet_addr(ip.c_str());

        if (type == "server")
        {
            if (bind(mysock, (sockaddr*)(&sai), sizeof(sai)) == SOCKET_ERROR)
                die("Bind error");
                echo("Bind OK!");

            if (listen(mysock, 1) == SOCKET_ERROR)
                die("Listen error");
                echo("Listen OK!");
        }
    }
    bool connect()
    {
        if (type == "client")
        {
            echo("Connecting...");
            if (::connect(mysock, (sockaddr*)(&sai), sizeof(sai)) == SOCKET_ERROR)
                dief("Connect error!");
                echo("Connection complete!");
        }
        else
        {
            echo("Accepting...");
            if ((remsock = accept(mysock, NULL, NULL)) == INVALID_SOCKET)
                dief("Accept error!");
                echo("Accepted!");
        }
        return true;
    }
    ~engine_t() { closesocket(mysock); }
    bool write(const string &s)
    {
        int len = s.size();
        SOCKET to = type == "server" ? remsock : mysock;
        int f1 = send(to, (char*)(&len), sizeof(len), NULL);
        strcpy(buf, s.c_str());
        int f2 = send(to, buf, len + 1, NULL);
        return f1 == sizeof(int) && f2 == len + 1;
    }
    bool read(string &s)
    {
        int len = 0;
        SOCKET from = type == "server" ? remsock : mysock;
        int f1 = recv(from, (char*)(&len), sizeof(len), NULL);
        int f2 = recv(from, buf, len + 1, NULL);
        s = string(buf);
        return f1 == sizeof(int) && f2 == len + 1;
    }
};

#endif
