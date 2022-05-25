#pragma once
#include "../../ServerLib/CConnectionSuper.h"

class CStreamConnection : public CConnectionSuper
{
private:
	SOCKET m_FileServerSocket;

public:
	CStreamConnection(void);
	~CStreamConnection(void);


	void OnConnection();
	void OnRecv();
	void OnClose();
};