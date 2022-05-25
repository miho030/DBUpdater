#include "stdafx.h"
#include "StremClient.h"

#define PORT 58080
#define IP "127.0.0.1"


ST_WSA_INITIALIZER g_WSAInitialize;

void PrintUi()
{
	printf("#---------------------------------------------------#\n");
	printf("#              File Updater Client Started          #\n");
	printf("#---------------------------------------------------#\n");
}


int main()
{
	PrintUi();
	printf("hi my client, file updater!!\n");


	CStremClient client;

	/*  Create socket & Connect to server  */
	client.ConnectServer(IP, PORT);
	/*  Download the file from server  */
	client.Downloader();
	/*  exit connection  */
	client.CloseConnection();
	

	return 0;
}