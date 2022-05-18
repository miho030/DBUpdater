#pragma once

class CClientSock
{
private:

public:
	CClientSock()
	{
		
	
	}
	~CClientSock() {};

	SOCKET hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
};