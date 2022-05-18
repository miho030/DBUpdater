#include "stdafx.h"
#include "../PMS_Server_Sock/CServerSock.h"
#include "../PMS_Server_Sock/CConnSock.h"


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



/*
* DWORD WINAPI ConnectionThreadCaller(void* pContext);
*
DWORD __stdcall ConnectionThreadCaller(void* pContext)
{
	CConnectionSock* pConnection = (CConnectionSock*)pContext;
	return pConnection->ConnectionThread(pContext);
}
*/

/*
DWORD WINAPI DBSender(LPVOID socket)
{
	CConnSock *conn = (CConnSock*)socket;

	printf("[+] server connection established.\n");



	// ---------------------------------------------------
	conn->m_recv(socket);

	Sleep(1000);
	conn->m_send(socket);
	
	conn->closeSock(socket);
	// ---------------------------------------------------
	return 0;
}
*/

DWORD WINAPI ConnectionThread(void* pContext)
{
	SOCKET hConnectionSock = (SOCKET)pContext;
	CConnSock* conn = 0;
	
	conn->m_recv(hConnectionSock);
	conn->m_send(hConnectionSock);
	conn->closeSock(hConnectionSock);
	
	return 0;
}


int main() // 얘가 ServerThread 역활
{
	
	printf("#---------------------------------------------------#\n");
	printf("#                     SERVER SIDE                    \n");
	printf("#---------------------------------------------------#\n\n");

	CServerSock server;
	CConnSock conn;
	
	int nRet = 0;

	while (true)
	{

		try
		{
			conn.m_hConnectionSock = server.Accept();
			if (SOCKET_ERROR == conn.m_hConnectionSock)
			{ 
				throw std::exception("Failed to create accept socket.");
				return -1;
			}
			if (INVALID_SOCKET != conn.m_hConnectionSock)
			{
				Sleep(2500);
				// 원하는 기능 불러오기
				HANDLE ThreadSock = CreateThread(nullptr, 0, ConnectionThread, (LPVOID)&conn.m_hConnectionSock, 0, nullptr);
				if (ThreadSock == 0)
				{
					int nErrorCode = ::WSAGetLastError();
					printf("[ERROR] WINAPI:CreateThread |   `Failed to create server thread`, ErrorCode: `%d`\n", nErrorCode);
					return -1;
				}
				WaitForSingleObject(ThreadSock, INFINITE);

			}
			
		}
		catch (const std::exception& Err)
		{
			printf("[ERROR] PmsMain:socketErr |   `%s`, ErrorCode: `%d`\n", Err.what(), WSAGetLastError());
		}

	}
	system("pause");
	return 0;
}