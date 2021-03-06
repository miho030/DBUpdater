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

	std::vector<CConnectionSuper*> m_vecConnectionInstance;
	std::queue<CConnectionSuper*> m_queReady;
	std::set<CConnectionSuper*> m_setConnected;
	std::queue<CConnectionSuper*> m_queDiscon;

	DWORD WINAPI DisconnectThread(LPVOID pContext);

public:
	DWORD WINAPI AcceptThread(LPVOID pContext);
	int StartUp(ST_SERVER_INIT);
	void ShutDown();
	DWORD WINAPI QueueHandler(SOCKET Handler);
	void Broadcast();
};