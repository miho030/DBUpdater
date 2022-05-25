#pragma once
class CChatClient
{
	SOCKET m_ClientSocket;

public:
	CChatClient(void);
	~CChatClient(void);

	int ConnectServer(const char* IPaddr, unsigned short wport);
	void CloseConnection(void);

	int Send(const char* pData, int pSize);
	int Recv(char* pBuf, int pSize);

};

