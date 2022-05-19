#include "stdafx.h"
#include "../ServerLib/CServer.h"

#define PORT 58580


int main()
{
	ST_SERVER_INIT init;

	CServer server;
	server.StartUp(init);


	printf("\n[INFO] ServerMain() has been successfully worked.\n\n");
	return 0;
}