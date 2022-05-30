#include "stdafx.h"
#include "StremClient.h"

#define PORT 58080
#define IP "172.30.100.21"


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

	CStreamClient client;

	/*  Create socket & Connect to server  */
	client.ConnectServer(IP, PORT);
	/*  Download the file from server  */
	client.Downloader();
	/*  exit connection  */
	client.CloseConnection();


	system("pause");
	return 0;
}