#pragma once
#include "CServer.h"

class CConnectionSuper
{

private:
	CServer *m_pServer;
	
	SOCKET m_Socket;
	HANDLE m_ConnThreadHandler;


public:
	CConnectionSuper(void);
	virtual ~CConnectionSuper(void);


	DWORD ConnectionThread();
	int Establish(SOCKET acceptedSocket, CServer* pServer);
	int Send(const char* pData, size_t tSize);
	int Recv(char* pBuffer, size_t BufferSize);

	virtual void OnConnection() = 0;
	virtual void OnRecv() = 0;
	virtual void OnClose() = 0;
};