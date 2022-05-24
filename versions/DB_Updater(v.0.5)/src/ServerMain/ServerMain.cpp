#include "stdafx.h"
#include "../ServerLib/CServer.h"
#include "../ServerLib/CConnectionSuper.h"
#include "../ServerFunc/CStreamConnection.h"

#define PORT 58080

int main(void)
{
	
	ST_SERVER_INIT init;
	init.wport = PORT;
	

	init.connections.push_back(new CStreamConnection());
	init.connections.push_back(new CStreamConnection());
	init.connections.push_back(new CStreamConnection());
	init.connections.push_back(new CStreamConnection());
	init.connections.push_back(new CStreamConnection());


	CServer server;
	server.StartUp(init);

	
	return 0;
}