#include "stdafx.h"
#include "ChatClient.h"


CChatClient::CChatClient(void)
{
}

CChatClient::~CChatClient(void)
{
}



int CChatClient::ConnectServer(const char* IPaddr, unsigned short wport)
{
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	addr.sin_port = htons(wport);
	addr.sin_addr.s_addr = inet_addr(IPaddr);
	addr.sin_family = AF_INET;
	return connect(m_ClientSocket, (sockaddr*)&addr, sizeof(addr));
}


/*  Send Recv  */
int CChatClient::Send(const char* sData, int sSize)
{
	return ::send(m_ClientSocket, sData, sSize, 0);
}

int CChatClient::Recv(char* rData, int rSize)
{
	return ::recv(m_ClientSocket, rData, rSize, 0);
}



/*  Close Socket  */
void CChatClient::CloseConnection(void)
{
	closesocket(m_ClientSocket);
	return;
}