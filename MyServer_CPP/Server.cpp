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
            cout << "WSAStartup: ����ͨ�����²��������ϵͳû׼����! " << endl;
            break;
        case WSAVERNOTSUPPORTED:
            cout << "WSAStartup: Socketʵ���ṩ�汾��socket��Ҫ�İ汾����! " << endl;
            break;
        case WSAEINPROGRESS:
            cout << "WSAStartup: һ��������Socket�������ڽ���! " << endl;
            break;
        case WSAEPROCLIM:
            cout << "WSAStartup: Socket��ʵ�ֳ���Socket֧�ֵ�����������! " << endl;
            break;
        case WSAEFAULT:
            cout << "WSAStartup: lpWSAData��������һ���Ϸ���ָ��! " << endl;
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
        cout << "socket: �����׽���ʧ��! �������: " << WSAGetLastError() << endl;
        WSACleanup(); //ע��WinSock��̬���ӿ�
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
        cout << "bind: ��ʧ��! �������: " << WSAGetLastError() << endl;
        closesocket(sock_server); //�ر��������׽���
        WSACleanup();             //ע��WinSock��̬���ӿ�
        getchar();
        return 0;
    }

    result = listen(sock_server, 0);
    if (result != 0)
    {
        cout << "listen: ����ʧ��! �������: " << WSAGetLastError() << endl;
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
            cout << "accept: ����ʧ��! �������: " << WSAGetLastError() << endl;
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
            cout << "send: ������Ϣʧ��! �������: " << WSAGetLastError() << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            continue;
        }
        else if (size == 0)
        {
            cout << "send: �ͻ����ѹر�����! " << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            continue;
        }
        else
        {
            cout << "send: ������Ϣ�ɹ�! " << endl;
        }

        size = recv(sock_server, msg, sizeof(msg), 0);
        if (size == SOCKET_ERROR)
        {
            cout << "recv: ������Ϣʧ��! �������: " << WSAGetLastError() << endl;
            closesocket(newsock);
            closesocket(sock_server);
            WSACleanup();
            getchar();
            return 0;
        }
        else if (size == 0)
        {
            cout << "recv: �ͻ����ѹر�����! " << endl;
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