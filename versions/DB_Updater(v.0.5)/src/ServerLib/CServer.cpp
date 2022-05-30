#include "stdafx.h"
#include "CServer.h"
#include "CConnectionSuper.h"

ST_WSA_INITIALIZER g_WSAInitialize;

/* Class Creater & Distructor */
CServer::CServer(void)
{
}
CServer::~CServer(void)
{
}


/*  for test func  */
void printUi(std::string funcName)
{
	std::string nFunc = "";

	if (funcName == "ST")
	{  nFunc = "StartUp() Initiated";  }
	if (funcName == "AT")
	{  nFunc = "AcceptThread() Initiated";  }

	printf("#---------------------------------------------------#\n");
	printf("                 %s               \n", nFunc.c_str());
	printf("#---------------------------------------------------#\n");

	Sleep(800);
}



/* Function Caller */
DWORD WINAPI AcceptThreadCaller(void* pContext)
{
	CServer* AccFuncCaller = (CServer*)pContext;
	return AccFuncCaller->AcceptThread();
}

DWORD WINAPI DisConnThreadCaller(void* pContext)
{
	CServer* DisFuncCaller = (CServer*)pContext;
	return DisFuncCaller->DisconnectThread();
}


/* Startup Function Start */
int CServer::StartUp(ST_SERVER_INIT init)
{
	printUi("ST");
	int nRet = 0; // for error handle



	/* --------    Conn Instance synchronizer    ---------- */
	try
	{
		m_vecConnectionInstance = init.connections;

		if (!m_vecConnectionInstance.empty())
		{
			for (CConnectionSuper* nConnInstance : m_vecConnectionInstance)
			{
				m_queReady.push(nConnInstance);
			}
		}
		else
		{
			throw std::exception("Create new connection Instance failure.");
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] ServerErr|Connection  `%s`", ErrMsg.what());
		return -1;
	}



	/* --------------    socket create    ------------------- */
	try
	{
		m_ListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SOCKET_ERROR == m_ListenSocket)
		{  throw std::exception("Create socket failure");  }
		if (INVALID_SOCKET == m_ListenSocket)
		{  throw std::exception("Invalid socket created");  }
		else 
		{  printf("[INFO] Successfully create new socket.\n");  }
	}
	catch (const std::exception& ErrMsg)
	{
		printf("*	[ERROR] WINAPI:socket() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}
	
	

	/* --------------    socket bind    ------------------- */
	try
	{
		sockaddr_in service;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons(init.wport);


		nRet = ::bind(m_ListenSocket, (sockaddr*)&service, sizeof(service));
		if (SOCKET_ERROR == nRet)
		{  throw std::exception("Socket bind failure");  }
		else { printf("[INFO] Successfully binded socket.\n"); }
	
	}
	catch (const std::exception& ErrMsg)
	{
		printf("*	[ERROR] WINAPI:bind() | %s, \n\tErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}



	/* --------------    socket listen    ----------------- */
	try
	{
		nRet = ::listen(m_ListenSocket, 200);
		if (SOCKET_ERROR == nRet)
		{  throw std::exception("Create listen socket failure");  }
		else
		{
			printf("[INFO] Sucessfully load server socket processes.\n");
		}
	}
	catch (const std::exception& ErrMsg)
	{
		printf("*	[ERROR] WINAPI:listen() | `%s`, ErrorCode : %d", ErrMsg.what(), WSAGetLastError());
		return -1;
	}
	

	/* --------------    Queue Handler    ----------------- */
	m_AcceptHandler = CreateThread(nullptr, 0, AcceptThreadCaller, this, 0, nullptr);
	{
		WaitForSingleObject(m_AcceptHandler, INFINITE);
	}

	m_DisconnHandler = CreateThread(nullptr, 0, DisConnThreadCaller, this, 0, nullptr);
	{
		WaitForSingleObject(m_DisconnHandler, INFINITE);
	}

	printf("[INFO] Server Execution Successful.\n");
	return 0;
}




DWORD CServer::AcceptThread()
{
	printf("\n");
	printUi("AT");
	printf("[INFO] Waiting for client connection ... \n");


	sockaddr remoteInfo;
	int nRemoteInfoSize = (int)sizeof(remoteInfo);

	/* -----------    Accept Error Handle    -------------- */
	while (true)
	{
		SOCKET newConnectionSock = ::accept(m_ListenSocket, &remoteInfo, &nRemoteInfoSize);

		/*  Accept Socket Error Handle  */
		try
		{
			if (INVALID_SOCKET == newConnectionSock)
			{  throw std::exception("Client socket accept failure");  }
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] WINAPI:accept() |  `%s`, ErrorCode: `%d`\n", ErrMsg, WSAGetLastError());
		}

		/*  Load Establish() & OnConnection() */
		try
		{
			if (!m_queReady.empty())
			{
				// Ready Queue->SetConnected
				CConnectionSuper* newConnection = m_queReady.front();
				m_queReady.pop();


				// move to connedted pool
				m_setConnected.insert(newConnection);
				
				// Load Establish() & OnConnection()
				int nRet = newConnection->Establish(newConnectionSock, this);
				printf("\n\n* * *\n\n");
				if (nRet == -1)
				{  throw std::exception("Server has been failed to accept client socket.");  }
			}
			else
			{  throw std::exception("Server has full connection.");  }
		}
		catch (const std::exception& ErrMsg)
		{
			printf("[ERROR] Server|Connection  `%s`", ErrMsg.what());
		}
		/*  end of while  */
	}

	return 0;
}

void CServer::Disconnect(CConnectionSuper* pConnection)
{
	m_setConnected.erase(pConnection);
	m_queDiscon.push(pConnection);
	DisconnectThread();
}

int CServer::DisconnectThread()
{
	
	/* -----------    Disconn Error Handle    ------------- */
	while (true)
	{
		if (m_queDiscon.empty())
		{
			break;
			return 0;
		}
		while (!m_queDiscon.empty())
		{
			// DinConn Queue->Ready Queue
			CConnectionSuper* DisConnInstance = m_queDiscon.front();
			m_queDiscon.pop();
			m_queReady.push(DisConnInstance);
		}
	}
}



void CServer::ShutDown()
{
	// wait until accept/disconnecte thread ars exit.
	WaitForSingleObject(m_AcceptHandler, INFINITE);
	WaitForSingleObject(m_DisconnHandler, INFINITE);

	/* close server socket */
	closesocket(m_ListenSocket);
	WSACleanup();
}



void CServer::Broadcast()
{
	return;
}