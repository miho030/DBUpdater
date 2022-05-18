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
		printf("[!] connection failed!\n\n");
		return -1;
	}
	printf("[+] successfully connected to server.\n");
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
	printf("[-] connection closed\n\n");

	return 0;
}



int main(void)
{
	printf("#---------------------------------------------------#\n");
	printf("#                     CLIENT SIDE                    \n");
	printf("#---------------------------------------------------#\n\n");

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