#include "stdafx.h"
#include "Client.h"

CClient::CClient(void)
{
}

CClient::~CClient(void)
{
}


int CClient::ConnectServer(const char* pszIP, unsigned short wPort)
{
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	addr.sin_port = htons(wPort);
	addr.sin_addr.s_addr = inet_addr(pszIP);
	addr.sin_family = AF_INET;
	return connect(m_ClientSocket, (sockaddr*)&addr, sizeof(addr));
}


/*  Send Recv  */
int CClient::Send(const char* sData, int sSize)
{
	return ::send(m_ClientSocket, sData, sSize, 0);
}

int CClient::Recv(char* rData, int rSize)
{
	return ::recv(m_ClientSocket, rData, rSize, 0);
}



/*  Close Socket  */
void CClient::CloseConnection(void)
{
	closesocket(m_ClientSocket);
	return;
}