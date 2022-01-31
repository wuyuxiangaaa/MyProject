#include <iostream>
#include <WinSock2.h>
using namespace std;

#pragma comment(lib, "WS2_32.lib")
//表示链接WS2_32.lib这个库。
#define PORT 1024

int main()
{
    SOCKET sock_server, newsock;
    struct sockaddr_in server_addr, client_addr;
    char msg[] = {"Hello client"};
    // 初始化 winsock2.dll[12/27/2017 MagicScaring]
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2); //生成版本号
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        cout << "加载 winsock.dll失败" << endl;
        return 0;
    }
    // 创建套接字 [12/27/2017 MagicScaring]
    if ((sock_server = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
    {
        cout << "创建套接字失败! 错误代码:" << WSAGetLastError() << endl;
        WSACleanup(); //注销WinSock动态链接库
        return 0;
    }
    // 填写需要绑定的本地地址 []
    int addr_len = sizeof(struct sockaddr_in);
    memset((void *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_server, (struct sockaddr *)&server_addr, addr_len) != 0)
    {
        cout << "绑定失败!错误代码:" << WSAGetLastError() << endl;
        closesocket(sock_server); //关闭已连接套接字
        WSACleanup();             //注销WinSock动态链接库
        return 0;
    }

    // 开始监听 [12/27/2017 MagicScaring]
    if (listen(sock_server, 0) != 0)
    {
        cout << "listen调用失败!错误代码:" << WSAGetLastError() << endl;
        closesocket(sock_server);
        WSACleanup();
        return 0;
    }
    else
    {
        cout << "listening...." << endl;
    }
    // 循环:接收连接请求并收发数据 [12/27/2017 MagicScaring]
    int size;
    while (true)
    {
        if ((newsock = accept(sock_server, (struct sockaddr *)&client_addr, &addr_len)) == INVALID_SOCKET)
        {
            cout << "accept 函数调用失败! 错误代码:" << WSAGetLastError() << endl;
            break;
        }
        else
        {
            cout << "成功接收到一个连接请求!" << endl;
            size = send(newsock, msg, sizeof(msg), 0);
            if (size == SOCKET_ERROR)
            {
                cout << "发送信息失败! 错误代码:" << WSAGetLastError() << endl;
                closesocket(newsock);
                continue;
            }
            else if (size == 0)
            {
                cout << "对方已关闭连接" << endl;
                closesocket(newsock);
                continue;
            }
            else
            {
                cout << "信息发送成功" << endl;
            }
            size = recv(newsock, msg, sizeof(msg), 0);
            if (size == SOCKET_ERROR)
            {
                cout << "接收信息失败！ 错误代码：" << WSAGetLastError() << endl;
                closesocket(newsock);
                WSACleanup();
                return 0;
            }
            else if (size == 0)
            {
                cout << "对方已关闭连接" << endl;
                closesocket(newsock);
                WSACleanup();
                return 0;
            }
            else
            {
                cout << "The message from Client:" << msg << endl;
            }
        }
    }

    return 0;
}