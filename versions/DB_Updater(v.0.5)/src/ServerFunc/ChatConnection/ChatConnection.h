#pragma once
#include "../../ServerLib/CConnectionSuper.h"

class CChatConnection : public CConnectionSuper
{

public:
	CChatConnection(void);
	~CChatConnection(void);
	

	void OnRecv();
	void OnConnection();
	void OnClose();
};