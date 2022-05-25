#pragma once
#include "stdafx.h"


class CStremClient
{
private:
	SOCKET m_ClientSocket;
public:
	CStremClient(void);
	~CStremClient(void);

	int ConnectServer(const char* IPaddr, unsigned short wport);
	int Downloader();
	void CloseConnection(void);

	int Send(const char* pData, int pSize);
	int Recv(char* pBuf, int pSize);
};

