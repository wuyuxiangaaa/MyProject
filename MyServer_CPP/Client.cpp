#include <winsock2.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    SOCKET sock_client;
    WORD wVersionRequested = MAKEWORD(2, 2);
    struct sockaddr_in server_addr;
    int size_sockaddr_in = sizeof(struct sockaddr_in);
    char msg[1000];
    char msg_send[1000];
    WSADATA WSAData;
    memset(msg, 0, sizeof(msg));
    int result;

    result = WSAStartup(wVersionRequested, &WSAData);
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

    sock_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_client == SOCKET_ERROR)
    {
        cout << "socket: 创建套接字失败! 错误代码: " << WSAGetLastError() << endl;
        WSACleanup();
        getchar();
        return 0;
    }

    const int PORT = 8080;
    char IP[20] = {"127.0.0.1"};
    memset((void *)&server_addr, 0, size_sockaddr_in);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);
    result = connect(sock_client, (struct sockaddr *)&server_addr, size_sockaddr_in);
    if (result == SOCKET_ERROR)
    {
        cout << "connect: 连接服务器失败! 错误代码: " << WSAGetLastError() << endl;
        closesocket(sock_client);
        WSACleanup();
        getchar();
        return 0;
    }

    cout << "Succeed in connecting with the server! " << endl;

    while (true)
    {
        int size = recv(sock_client, msg, sizeof(msg), 0);
        if (size == SOCKET_ERROR)
        {
            cout << "recv: 接收消息失败! 错误代码: " << WSAGetLastError() << endl;
            closesocket(sock_client);
            WSACleanup();
            getchar();
            return 0;
        }
        else if (size == 0)
        {
            cout << "recv: 服务器已关闭连接! " << endl;
            closesocket(sock_client);
            WSACleanup();
            getchar();
            return 0;
        }
        else
        {
            cout << "recv: There is a message form the server: " << msg << endl;
        }

        cout << "Please write down what you want to say: " << endl;
        cin >> msg_send;

        size = send(sock_client, msg_send, sizeof(msg_send), 0);
        if (size == SOCKET_ERROR)
        {
            cout << "send: 发送消息失败! 错误代码: " << WSAGetLastError() << endl;
            closesocket(sock_client);
            WSACleanup();
            getchar();
            return 0;
        }
        else if (size == 0)
        {
            cout << "send: 服务器已关闭连接! " << endl;
            closesocket(sock_client);
            WSACleanup();
            getchar();
            return 0;
        }
        else
        {
            cout << "send: Succeed in send a message! " << endl;
        }
    }

    getchar();
    return 0;
}