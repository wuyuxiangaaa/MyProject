#include <winsock2.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    struct sockaddr_in server_addr, client_addr;
    WSADATA WSAData;
    SOCKET sock_server;
    char msg[1000] = {"hello!"};

    int result = WSAStartup(wVersionRequested, &WSAData);
    if (result != 0)
    {
        switch (result)
        {
        case WSASYSNOTREADY:
            cout << "WSAStartup: 网络通信中下层的网络子系统没准备好! " << endl;
            break;
        case WSAVERNOTSUPPORTED:
            cout << "WSAStartup: Socket实现提供版本和socket需要的版本不符! " << endl;
            break;
        case WSAEINPROGRESS:
            cout << "WSAStartup: 一个阻塞的Socket操作正在进行! " << endl;
            break;
        case WSAEPROCLIM:
            cout << "WSAStartup: Socket的实现超过Socket支持的任务数限制! " << endl;
            break;
        case WSAEFAULT:
            cout << "WSAStartup: lpWSAData参数不是一个合法的指针! " << endl;
            break;
        default:
            cout << "WSAStartup: Unknown errors! " << endl;
        }
        getchar();
        return 0;
    }

    sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_server == SOCKET_ERROR)
    {
        cout << "socket: 创建套接字失败! 错误代码: " << WSAGetLastError() << endl;
        WSACleanup(); //注销WinSock动态链接库
        getchar();
        return 0;
    }

    const int PORT = 8080;
    const char IP[20] = {"127.0.0.1"};
    int size_sockaddr_in = sizeof(struct sockaddr_in);
    memset((void *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);
    result = bind(sock_server, (struct sockaddr *)&server_addr, size_sockaddr_in);
    if (result != 0)
    {
        cout << "bind: 绑定失败! 错误代码: " << WSAGetLastError() << endl;
        closesocket(sock_server); //关闭已连接套接字
        WSACleanup();             //注销WinSock动态链接库
        getchar();
        return 0;
    }

    result = listen(sock_server, 0);
    if (result != 0)
    {
        cout << "listen: 监听失败! 错误代码: " << WSAGetLastError() << endl;
        closesocket(sock_server);
        WSACleanup();
        getchar();
        return 0;
    }

    cout << "Start listening... " << endl;

    while (true)
    {
        SOCKET newsock;
        newsock = accept(sock_server, (struct sockaddr *)&client_addr, &size_sockaddr_in);
        if (newsock != INVALID_SOCKET)
        {
            cout << "accept: 接受失败! 错误代码: " << WSAGetLastError() << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            getchar();
            return 0;
        }
        cout << "Succeed in accept a request! " << endl;
        int size = send(sock_server, msg, sizeof(msg), 0);
        if (size == -1)
        {
            cout << "send: 发送信息失败! 错误代码: " << WSAGetLastError() << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            continue;
        }
        else if (size == 0)
        {
            cout << "send: 客户端已关闭连接! " << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            continue;
        }
        else
        {
            cout << "send: 发送消息成功! " << endl;
        }

        size = recv(sock_server, msg, sizeof(msg), 0);
        if (size == SOCKET_ERROR)
        {
            cout << "recv: 接收消息失败! 错误代码: " << WSAGetLastError() << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            getchar();
            return 0;
        }
        else if (size == 0)
        {
            cout << "recv: 客户端已关闭连接! " << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            getchar();
            return 0;
        }
        else
        {
            cout << "recv: There is a message: " << msg << endl;
        }
    }

    getchar();
    return 0;
}