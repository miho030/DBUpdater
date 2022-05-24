#pragma once

class CConnectionSuper;

struct ST_SERVER_INIT
{
	WORD wport;
	std::vector<CConnectionSuper*> connections;
};


class CServer
{
private:
	SOCKET m_ListenSocket;
	
	HANDLE m_AcceptHandler;
	HANDLE m_DisconnHandler;

	std::vector<CConnectionSuper*> m_vecConnectionInstance;
	std::queue<CConnectionSuper*> m_queReady;
	std::set<CConnectionSuper*> m_setConnected;
	std::queue<CConnectionSuper*> m_queDiscon;


public:
	CServer(void);
	~CServer(void);


	int StartUp(ST_SERVER_INIT);
	DWORD AcceptThread();
	
	void Disconnect(CConnectionSuper* pConnection);
	int DisconnectThread();
	void ShutDown();
	
	void Broadcast();
};