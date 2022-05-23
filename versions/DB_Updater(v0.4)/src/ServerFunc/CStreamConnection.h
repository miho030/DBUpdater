#pragma once
#include "../ServerLib/CConnectionSuper.h"

class CStreamConnection : public CConnectionSuper
{
private:

public:
	void OnRecv();
	void OnConnection();
	void OnClose();
};