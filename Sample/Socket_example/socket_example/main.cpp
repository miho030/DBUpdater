#include <Windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

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

DWORD WINAPI ConnectionThread(void* pContext)
{
	SOCKET hConnectionSock = (SOCKET)pContext;

	printf("server connection established.\n");

	{
		int nLength = 0;
		::recv(hConnectionSock, (char*)&nLength, sizeof(nLength), 0);

		std::string strMsg;
		strMsg.resize(nLength);
		::recv(hConnectionSock, (char*)strMsg.c_str(), nLength, 0);

		printf("server recved msg `%s`.\n", strMsg.c_str());
	}

	Sleep(1000);
	{
		std::string strMsg = "Hello client??";
		int nLength = strMsg.length();
		::send(hConnectionSock, (const char*)&nLength, sizeof(nLength), 0);
		::send(hConnectionSock, strMsg.c_str(), strMsg.length(), 0);
	}

	closesocket(hConnectionSock);
	printf("connection closed\n");
	return 0;
}

DWORD WINAPI ServerThread(void* pContext)
{
	SOCKET hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hListenSocket)
	{
		int nErrorCode = ::WSAGetLastError();
		return -1;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(55000);
	int nRet = ::bind(hListenSocket, (sockaddr*)&service, sizeof(service));
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	nRet = ::listen(hListenSocket, 200);
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	while (true)
	{
		sockaddr remoteInfo;
		int nRemoteInfoSize = (int)sizeof(remoteInfo);
		SOCKET hConnectionSock = ::accept(hListenSocket, &remoteInfo, &nRemoteInfoSize);
		if (INVALID_SOCKET == hConnectionSock)
		{
			return -1;
		}

		HANDLE hThread = CreateThread(nullptr, 0, ConnectionThread, (LPVOID)hConnectionSock, 0, nullptr);
	}

	return 0;
}

DWORD WINAPI ClientThread(void* pContext)
{
	SOCKET hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hClientSocket)
	{
		return -1;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(55000);
	int nRet = ::connect(hClientSocket, (sockaddr*)&clientService, sizeof(clientService));
	if (SOCKET_ERROR == nRet)
	{
		int nErrorCode = WSAGetLastError();
		return -1;
	}
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

		printf("client recved msg `%s`.\n", strMsg.c_str());
	}

	char szBuffer[10];
	int nRecvedSize = ::recv(hClientSocket, szBuffer, 10, 0);
	::closesocket(hClientSocket);

	return 0;
}

int main(void)
{
	DWORD dwThreadId = 0;
	HANDLE hServerThread = ::CreateThread(nullptr, 0, ServerThread, nullptr, 0, &dwThreadId);

	Sleep(200);

	std::vector<HANDLE> vecClientThread;
	for (int i = 0; i < 5; i++)
	{
		HANDLE hClientThread = CreateThread(nullptr, 0, ClientThread, nullptr, 0, nullptr);
		vecClientThread.push_back(hClientThread);
		Sleep(500);
	}

	for (size_t i = 0; i < vecClientThread.size(); i++)
		WaitForSingleObject(vecClientThread[i], INFINITE);

	return 0;
}