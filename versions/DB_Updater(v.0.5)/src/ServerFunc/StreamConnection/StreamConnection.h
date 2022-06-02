#pragma once
#include "../../ServerLib/CConnectionSuper.h"

class CStreamConnection : public CConnectionSuper
{
private:
	SOCKET m_FileServerSocket;
	std::string tm_Date;
	std::string tm_Time;

public:
	CStreamConnection(void);
	~CStreamConnection(void);


	const char* GetFileName(const char* file_path);
	void TimeStamp();

	void OnConnection();
	void OnRecv();
	void OnClose();
};