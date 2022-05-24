#include "stdafx.h"
#include "CStreamConnection.h"

CStreamConnection::CStreamConnection(void)
{
}

CStreamConnection::~CStreamConnection(void)
{
}

void CStreamConnection::OnRecv()
{
	printf("[INFO] Connected to Client.\n");
    OnConnection();
}

void CStreamConnection::OnConnection()
{
    char szMsg[100 + 1];
    int nRecvSize = Recv(szMsg, 100);
    szMsg[nRecvSize] = 0;
    printf("[INFO] message from client :   %s\n", szMsg);

    Send("Dude, Here is your file.", 24);
    printf("Error Log : %d\n", WSAGetLastError());
    
}

void CStreamConnection::OnClose()
{
    printf("[INFO] Connection Closed.\n\n");
}
