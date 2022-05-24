#include "stdafx.h"
#include "CServer.h"
#include "CConnectionSuper.h"

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

DWORD WINAPI CServer::AcceptThread(LPVOID pContext)
{
	printUi("AT");
	CServer& server = *(CServer*)pContext;

	sockaddr remoteInfo;
	int nRemoteInfoSize = (int)sizeof(remoteInfo);
	SOCKET newConnectionSock = ::accept(m_ListenSocket, &remoteInfo, &nRemoteInfoSize);
	
	while (true)
	{
		try //accept error handle
		{
			if (INVALID_SOCKET == newConnectionSock)
			{ throw std::exception("Client socket accept failure"); }
			else
			{ printf("[INFO] Successfully created accept socket.\n"); }
			// 
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] WINAPI:accept() |  `%s`, ErrorCode: `%d`\n", ErrMsg, WSAGetLastError());
			return -1;
		}
	}
	return 0;
}


DWORD WINAPI CServer::DisconnectThread(LPVOID pContext)
{
	CServer server = *(CServer*)pContext;

	return 0;
}

void CServer::ShutDown()
{
}


DWORD WINAPI CServer::QueueHandler(SOCKET Handler)
{
	CServer server;
	SOCKET SockHandler = Handler;

	/* acceptThread에서 얘가 while문 쓰레드로 돌아갈 수 있도록
		Ready Queue가 없을 때 DisConn Queue가 빌때까지 Ready Queue를 채운다
	*/


	try // DisConn -> Ready
	{
		if (!m_queReady.empty())
		{
			/*
				1. Ready queue 앞에 하나 가져오기
				2. ConnectionSuper의 Establish() 함수로 보내기
				3. 해당 connection의 ready queue를 connected queue로 보내기
			*/
			CConnectionSuper* newConnection = m_queReady.front();
			newConnection->Establish(SockHandler, this);
			m_setConnected.insert(newConnection);
		}
		else
		{
			throw("Server has full connection.");
			DisconnectThread(&SockHandler);
			throw("Client Disconnected.");
			while (!m_queReady.empty())
			{
				Sleep(1000);
				if (!m_queDiscon.empty())
				{
					CConnectionSuper* disConnection = m_queDiscon.front();
					m_queDiscon.pop();
					m_queReady.push();
				}
			}
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] Queue|Connection  `%s`. ", ErrMsg.what());
	}

	return 0;
}

void CServer::Broadcast()
{
}