#include "stdafx.h"
#include "../ServerLib/CServer.h"

#define PORT 58580


int main()
{

	CServer server;
	ST_SERVER_INIT init;

	init.wport = PORT;


		/* 이 부분이 main에서 인스턴스 가지고 On~함수들로 넘겨줄 수 있는 부분
		std::vector<CConnectionSuper*> vecConnPool;

		CConnectionSuper* connection = new StreamConnection;
		vecConnPool.push_back(connection);
		*/

	server.StartUp(init);


	printf("\n[INFO] ServerMain() has been successfully worked.\n\n");
	return 0;
}