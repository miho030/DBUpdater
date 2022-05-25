#include "../stdafx.h"
#include "ChatConnection.h"

CChatConnection::CChatConnection(void)
{
}

CChatConnection::~CChatConnection(void)
{
}



void CChatConnection::OnConnection()
{
	printf("[INFO] Connected to Client.\n");
}


void CChatConnection::OnRecv()
{
	char szMsg[100 + 1];
	int nRecvSize = Recv(szMsg, 100);
	szMsg[nRecvSize] = 0;
	printf("[INFO] message from client :   %s\n", szMsg);

	Send("Here is file", 12);

	printf("Error Log : %d\n", WSAGetLastError());
}

void CChatConnection::OnClose()
{
    printf("[INFO] Connection Closed.\n");
}
