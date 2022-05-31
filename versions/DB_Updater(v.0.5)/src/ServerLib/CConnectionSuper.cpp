#include "stdafx.h"
#include "CConnectionSuper.h"


/* Class Creater & Distructor */
CConnectionSuper::CConnectionSuper(void)
{
}
CConnectionSuper::~CConnectionSuper(void)
{
}


/* -------------	Connect Fund    --------------- */
DWORD WINAPI ConnectionThreadCaller(LPVOID pContext)
{
	CConnectionSuper* ConnFuncCaller = (CConnectionSuper*)pContext;
	return ConnFuncCaller->ConnectionThread();
}



DWORD CConnectionSuper::ConnectionThread()
{
	OnConnection();
	OnRecv();

	m_pServer->Disconnect(this);
	OnClose();
	Sleep(100);
	
	return 0;
}


/* --------------	Crate Thread    ---------------- */
int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_pServer = pServer;
	m_Socket = acceptedSocket;
	m_ConnThreadHandler = CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);

	/*  Connection Thread  Error Handle  */
	try
	{
		if (NULL == m_ConnThreadHandler)
		{  throw std::exception("Create Connection Thread failure.");  }
		
		/* WaitForSingObject failed  */
		int dw = WaitForSingleObject(m_ConnThreadHandler, 2000);
		if (dw == WAIT_FAILED)
		{  throw std::exception("Thread waiting failure.");  }
	}
	catch (const std::exception& ErrMsg)
	{
		printf("[ERROR] WINAPI|Thread  `%s`", ErrMsg.what());
		return -1;
	}

	return 0;
}


/*  Send the file func  */
int CConnectionSuper::Send(const char* sData, size_t sSize)
{
	return ::send(m_Socket, sData, sSize, 0);
}


/*  Recv the file func  */
int CConnectionSuper::Recv(char* rData, size_t rSize)
{
	return ::recv(m_Socket, rData, rSize, 0);
}

SOCKET CConnectionSuper::getSocket()
{

	return m_Socket;
}
