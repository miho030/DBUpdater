#include "stdafx.h"
#include "CServerSock.h"

SOCKET CServerSock::Accept()
{
	sockaddr remoteInfo;
	int nRemoteInfoSize = (int)sizeof(remoteInfo);
	SOCKET m_hConnectionSock = ::accept(m_hListenSocket, &remoteInfo, &nRemoteInfoSize);
	if (INVALID_SOCKET == m_hConnectionSock)
	{
		int nErrorCode = ::WSAGetLastError();
		printf("[ERROR] WINAPI:accept() |  `Socket accept failure`, ErrorCode: `%d`\n", nErrorCode);
		return -1;
	}
	return m_hConnectionSock;
}
