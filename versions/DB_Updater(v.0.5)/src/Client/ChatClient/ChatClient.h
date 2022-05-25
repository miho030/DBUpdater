#pragma once
class CClient
{
	SOCKET m_ClientSocket;

public:
	CClient(void);
	~CClient(void);

	int ConnectServer(const char* IpAddr, unsigned short wPort);
	void CloseConnection(void);

	int Send(const char* pData, int pSize);
	int Recv(char* pBuf, int pSize);

};

