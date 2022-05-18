#pragma once
#include <WinSock2.h>

class CConnSock
{
private:

public:
	SOCKET m_hConnectionSock;
	void m_recv(SOCKET RecvSock);
	void m_send(SOCKET SendSock);
	void closeSock(SOCKET CloseSock);
};