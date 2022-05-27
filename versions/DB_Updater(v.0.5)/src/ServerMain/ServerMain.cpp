#include "stdafx.h"
#include "../ServerLib/CServer.h"
#include "../ServerLib/CConnectionSuper.h"
#include "../ServerFunc/ChatConnection/ChatConnection.h"
#include "../ServerFunc/StreamConnection/StreamConnection.h"

#define PORT 58080
#define MAXIMUM_CONN 5

int main(void)
{
	
	ST_SERVER_INIT init;
	init.wport = PORT;
	
	for (int count = 0; count < MAXIMUM_CONN; ++count)
	{
		init.connections.push_back(new CStreamConnection());
	}

	CServer server;
	server.StartUp(init);
	

	system("pause");
	return 0;
}