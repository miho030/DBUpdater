#include "stdafx.h"
#include "Client.h"

#define PORT 58080

ST_WSA_INITIALIZER g_WSAInitialize;

void PrintUi()
{
	printf("#---------------------------------------------------#\n");
	printf("#              Client Side process started          #\n");
	printf("#---------------------------------------------------#\n\n");
}

int ClientSend(void)
{
	CClient client;
	client.ConnectServer("127.0.0.1", PORT);
	printf("[INFO] Successfully connected to server.\n");

	client.Send("Hey, I want your file.", 22);

	printf("[INFO] msg sended.\n");

	char szBuffer[100 + 1];
	int nReadSize = client.Recv(szBuffer, 100);
	if (nReadSize < 0)
	{
		printf("[ERROR] Failed to recieve data from server! | ErrorCode: %d\n", WSAGetLastError());
		return -1;
	}
	szBuffer[nReadSize] = 0;

	printf("[INFO] Message from server :   %s\n", szBuffer);

	client.CloseConnection();
	return 0;
}


int main()
{
	PrintUi();
	printf("* --------------------------------------------------- *\n");


	while (true)
	{
		ClientSend();
	}
	
	system("pause");
	return 0;
}