#pragma once
#include "CServer.h"

class CConnectionSuper
{
private:
	CServer *m_pServer = 0;
	SOCKET m_Socket;
	void ConnectionThread();

public:
	int Establish(SOCKET acceptedSocket, CServer* server);
	int Send(const LPBYTE pData, size_t tSize);
	int Recv(LPBYTE pBuffer, size_t BufferSize);

	virtual void OnConnection() = 0;
	virtual void OnRecv() = 0;
	virtual void OnClsoe() = 0;
};