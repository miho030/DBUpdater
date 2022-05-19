#include "stdafx.h"
#include "CServer.h"
#include "CConnectionSuper.h"

#define PORT 58580
ST_WSA_INITIALIZER g_WSAInitialize;


void printUi(std::string funcName)
{
	std::string nFunc = "";
	
	if (funcName == "ST")
	{ nFunc = "StartUp() Initiated"; }
	if (funcName == "AT")
	{ nFunc = "AcceptThread() Initiated"; }

	printf("#---------------------------------------------------#\n");
	printf("                      %s                    \n", nFunc.c_str());
	printf("#---------------------------------------------------#\n\n");
	
	Sleep(800);
}


int CServer::StartUp(ST_SERVER_INIT)
{
	printUi("ST");


	ST_SERVER_INIT init;
	init.wport = PORT;

	int nRet = 0; // for error handle


	/* --------------    socket create    ------------------- */
	try // socket()
	{
		
		m_ListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SOCKET_ERROR == m_ListenSocket)
		{
			throw std::exception("Create socket failure");
		}
		if (INVALID_SOCKET == m_ListenSocket)
		{
			throw std::exception("Invalid socket created");
		}
		else 
		{ 
			printf("[INFO] Successfully create new socket.\n"); 
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:socket() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}
	
	// setup service
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(init.wport);



	/* --------------    socket bind    ------------------- */
	try // bind()
	{
		nRet = ::bind(m_ListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nRet)
		{
			throw std::exception("Socket bind failure");
		}
		else { printf("[INFO] Successfully binded socket.\n"); }
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:bind() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}


	/* --------------    socket bind    ------------------- */
	try // listen()
	{
		nRet = ::listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nRet)
		{
			throw std::exception("Create listen socket failure");
		}
		else
		{
			printf("[INFO] Successfully created listen socket.\n");
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:listen() | `%s`, ErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}

	return 0;
}

void CServer::AcceptThread()
{
	printUi("AT");


	CConnectionSuper* newConnection;
	
	sockaddr remoteInfo;
	int nRemoteInfoSize = (int)sizeof(remoteInfo);

	while (true)
	{
		SOCKET newConnectionSock = ::accept(m_ListenSocket, &remoteInfo, &nRemoteInfoSize);
		try // accept error handle
		{
			if (INVALID_SOCKET == newConnectionSock)
			{
				throw std::exception("Client socket accept failure");
			}
			else
			{
				printf("[INFO] Successfully created accept socket.\n");
			}
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] WINAPI:accept() |  `%s`, ErrorCode: `%d`\n", ErrMsg, WSAGetLastError());
			return;
		}

		/*
		newConnection = m_queReady.pop();
		newConnection->Establish(newConnectionSock, this);
		m_setConnected.insert(newConnection);
		*/
	}

}



void CServer::DisconnectThread()
{
}

void CServer::ShutDown()
{
}

void CServer::Broadcast()
{
}
