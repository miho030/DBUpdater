#include "stdafx.h"
#include "../Client_Sock/CClient.h"
#pragma comment(lib, "ws2_32.lib")

/* 코드 초안 */
/*
* change cmd console color //Usage : setColor(RED);
enum {  BLACK,DARK_BLUE,DARK_GREEN,DARK_SKYBLUE,DARK_RED,DARK_VOILET,DAKR_YELLOW,GRAY,DARK_GRAY,BLUE,GREEN,SKYBLUE,RED,VIOLET,YELLOW,WHITE,  };

void setColor(unsigned short text) {  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);  }
*/

struct ST_WSA_INITIALIZER
{
	WSAData wsa;
	ST_WSA_INITIALIZER(void)
	{
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ST_WSA_INITIALIZER(void)
	{
		WSACleanup();
	}
};
ST_WSA_INITIALIZER g_WSAInitialize;


DWORD WINAPI ClientThread(void* pContext)
{
	SOCKET hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hClientSocket)
	{
		int nErrorCode = ::WSAGetLastError();
		printf("[ERROR]  WINAPI:socket() |   Failed to creat socket.\n");
		printf("         ㄴ ErrorCode : %d\n\n", nErrorCode);
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(55000);
	int nRet = ::connect(hClientSocket, (sockaddr*)&clientService, sizeof(clientService));
	if (SOCKET_ERROR == nRet)
	{
		int nErrorCode = WSAGetLastError();
		printf("[ERROR] WINAPI:connect() |   Failed to connec to server.\n");
		printf("         ㄴ ErrorCode : %d\n\n", nErrorCode);
		if (10061 == nRet)
		{
			printf("[ERROR-INFO] WINAPI:connect() |  Date send refused by server.\n");
		}

		return -1;
	}
	else
	{
		printf("[INFO] successfully connected to server.\n");
		Sleep(500);

		{
			std::string strMsg = "Hello server??";
			int nLength = strMsg.length();
			::send(hClientSocket, (const char*)&nLength, sizeof(nLength), 0);
			::send(hClientSocket, strMsg.c_str(), strMsg.length(), 0);
		}

		{
			int nLength = 0;
			::recv(hClientSocket, (char*)&nLength, sizeof(nLength), 0);

			std::string strMsg;
			strMsg.resize(nLength);
			::recv(hClientSocket, (char*)strMsg.c_str(), nLength, 0);

			printf("	==> client recved msg `%s`.\n", strMsg.c_str());
		}

		char szBuffer[10];
		int nRecvedSize = ::recv(hClientSocket, szBuffer, 10, 0);
		::closesocket(hClientSocket);
		printf("[INFO] connection closed\n\n");
	}
	return 0;
}

int main()
{
	printf("#---------------------------------------------------#\n");
	printf("#                     CLIENT SIDE                    \n");
	printf("#---------------------------------------------------#\n\n");

	CClientSock client;


	Sleep(500);
	std::vector<HANDLE> vecClientThread;
	for (int i = 0; i < 5; i++)
	{
		HANDLE hClientThread = CreateThread(nullptr, 0, ClientThread, nullptr, 0, nullptr);
		vecClientThread.push_back(hClientThread);
		Sleep(500);
	}

	for (size_t i = 0; i < vecClientThread.size(); i++)
		WaitForSingleObject(vecClientThread[i], INFINITE);

	system("pause");
	return 0;
}