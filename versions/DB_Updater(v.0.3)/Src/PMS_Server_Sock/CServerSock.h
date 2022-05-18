#pragma once

class CServerSock
{
private:

public:
	CServerSock()
	{
		m_hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_hListenSocket)
		{
			int nErrorCode = ::WSAGetLastError();
			printf("[ERROR] WINAPI:socketErr |  `INVAILED SOCKET`, ErrorCode: `%d` \n", nErrorCode);
			return;
		}

		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(55000);
		int nRet = ::bind(m_hListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nRet)
		{
			int nErrorCode = ::WSAGetLastError();
			printf("[ERROR] WINAPI:bind() |  `Socket bind failure`, ErrorCode: `%d`\n", nErrorCode);
			return;
		}

		nRet = ::listen(m_hListenSocket, 200);
		if (SOCKET_ERROR == nRet)
		{
			int nErrorCode = ::WSAGetLastError();
			printf("[ERROR] WINAPI:listen() |  `Socket listen failure`, ErrorCode: `%d` \n", nErrorCode);
			return;
		}
	};
	~CServerSock() {};

	SOCKET m_hListenSocket;
	SOCKET Accept();
};
