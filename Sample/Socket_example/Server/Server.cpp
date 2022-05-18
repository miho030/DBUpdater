#include "stdfx.h"
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

	printf("[+] server connection established.\n");

	{
		int nLength = 0;
		::recv(hConnectionSock, (char*)&nLength, sizeof(nLength), 0);

		std::string strMsg;
		strMsg.resize(nLength);
		::recv(hConnectionSock, (char*)strMsg.c_str(), nLength, 0);

		printf("	==> server recved msg `%s`.\n", strMsg.c_str());
	}

	Sleep(1000);
	{
		std::string strMsg = "Hello client??";
		int nLength = strMsg.length();
		::send(hConnectionSock, (const char*)&nLength, sizeof(nLength), 0);
		::send(hConnectionSock, strMsg.c_str(), strMsg.length(), 0);
	}

	closesocket(hConnectionSock);
	printf("[-] connection closed\n\n");
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

	nRet = ::listen(hListenSocket, SOMAXCONN);
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

int main(void)
{
	printf("#---------------------------------------------------#\n");
	printf("#                     SERVER SIDE                    \n");
	printf("#---------------------------------------------------#\n\n");


	Sleep(200);
	DWORD dwThreadId = 0;
	HANDLE hServerThread = ::CreateThread(nullptr, 0, ServerThread, nullptr, 0, &dwThreadId);


	while (TRUE)
	{
		DWORD ret = WaitForSingleObject(hServerThread, INFINITE);
	
		if (ret == WAIT_FAILED)
			return 0;
		else if (ret == WAIT_ABANDONED)
		{
			ResetEvent(hServerThread);
			continue;
		}
		else if (WAIT_TIMEOUT)
		{ continue; }
		else
		{
			ResetEvent(hServerThread);
			printf("TESTESTE");
		}
	}
	  

	system("pause");
	return 0;
}