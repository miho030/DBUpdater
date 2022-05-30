#pragma once
#include "stdafx.h"


class CStreamClient
{
private:
	SOCKET m_ClientSocket;
public:
	CStreamClient(void);
	~CStreamClient(void);

	int ConnectServer(const char* IPaddr, unsigned short wport);
	int PreLoader(char* fNameBuf);
	int Downloader();
	int FILEnDirExistChecker(const char* fileDir);
	void CloseConnection(void);

	int Send(const char* pData, int pSize);
	int Recv(char* pBuf, int pSize);
};

