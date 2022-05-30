#pragma once
#include "../../ServerLib/CConnectionSuper.h"

class CStreamConnection : public CConnectionSuper
{
private:
	SOCKET m_FileServerSocket;

public:
	CStreamConnection(void);
	~CStreamConnection(void);

	char* GetFileName(char* file_path);
	void PreLoader();
	void Sender();
	
	void OnConnection();
	void OnRecv();
	void OnClose();
};