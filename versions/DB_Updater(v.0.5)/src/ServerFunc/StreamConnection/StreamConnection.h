#pragma once
#include "../../ServerLib/CConnectionSuper.h"

class CStreamConnection : public CConnectionSuper
{
public:
	CStreamConnection(void);
	~CStreamConnection(void);

	void OnRecv();
	void OnConnection();
	void OnClose();
};