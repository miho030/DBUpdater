#include "stdafx.h"
#include "CConnectionSuper.h"


void CConnectionSuper::ConnectionThread()
{
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* server)
{
	return 0;
}

int CConnectionSuper::Send(const LPBYTE pData, size_t tSize)
{
	return 0;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	return 0;
}