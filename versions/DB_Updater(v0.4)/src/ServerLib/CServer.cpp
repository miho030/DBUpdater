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


	try // queReady가 비어있는지 확인
	{ // 얘가 쓰레드로 계속 돌면서 큐를 확인하는 식으로 하면 좋을듯.
		
		//m_queReady.empty();
		/* 
		* 
		* STL tree 구조일 때는 insert / 큐,셋 은 push
		* 
		1. 레디큐에 자리가 있는지 확인
		2. 없으면 queDisconn -> pop() -> 레디큐.push_back()


		*/
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] Queue| `%s`", ErrMsg.what());
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
		try // accept error handle
		{
			if (INVALID_SOCKET == newConnectionSock)
			{ throw std::exception("Client socket accept failure"); }
			else
			{ printf("[INFO] Successfully created accept socket.\n"); }
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] WINAPI:accept() |  `%s`, ErrorCode: `%d`\n", ErrMsg, WSAGetLastError());
			return -1;
		}


		try // ReadyQ->ConnectedQ 에러 처리 관련
		{
			CConnectionSuper* newConnection = server.m_queReady.front(); // ready 큐 하나 가져오기
			newConnection->Establish(newConnectionSock, this); // CconnectionSuper의 Establish()로 보내기
			server.m_setConnected.insert(newConnection); // 연결 되면 connected 큐에 넣기
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] QUEUE|Connection  `%s`. ", ErrMsg.what());
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

void CServer::Broadcast()
{
}