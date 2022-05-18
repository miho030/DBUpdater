#include "stdafx.h"
#include "CServer.h"
#include "CConnectionSuper.h"


void CServer::AcceptThread()
{

	ST_WSA_INITIALIZER g_WSAInitialize;
	int nRet = 0;


	try
	{
		// socket()
		m_ListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SOCKET_ERROR == m_ListenSocket)
		{
			throw std::exception("Failed to create socket.");
			return;
		}
		if (INVALID_SOCKET == m_ListenSocket)
		{
			throw std::exception("Invalid socket created.");
			return;
		}
		else { printf("[INFO] sucess to socket\n"); }
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:socket() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		exit(1);
	}


	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(25740);

	try
	{
		nRet = ::bind(m_ListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nRet)
		{
			throw std::exception("Failed to bind.");
			return;
		}
		else { printf("[INFO] sucess to bind\n"); }
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:bind() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		exit(1);
	}

	try
	{
		nRet = ::listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nRet)
		{
			throw std::exception("Failed to start listen process.");
			return;
		}
		else
		{
			printf("");
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI:listen() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		exit(1);
	}
}

void CServer::DisconnectThread()
{
}

int CServer::StartUp(ST_SERVER_INIT)
{
	return 0;
}

void CServer::ShutDown()
{
}

void CServer::Broadcast()
{

}
