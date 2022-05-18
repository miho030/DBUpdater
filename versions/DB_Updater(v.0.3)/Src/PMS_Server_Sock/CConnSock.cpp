#include "stdafx.h"
#include "CConnSock.h"

void CConnSock::m_recv(SOCKET RecvSock)
{
	try
	{
		int nRecvRes = 0;
		int nLength = 0;

		nRecvRes = ::recv(RecvSock, (char*)&nLength, sizeof(nLength), 0);
		if (nRecvRes == SOCKET_ERROR)
		{
			if (nLength == 0) { throw std::exception("recieved message length is zero");  }
			else { throw std::exception("recieve length failure"); }
		}

		std::string strMsg;
		strMsg.resize(nLength);
		nRecvRes = ::recv(RecvSock, (char*)strMsg.c_str(), nLength, 0);
		if (nRecvRes == SOCKET_ERROR)
		{
			throw std::exception("recieve data failure");
		}

		printf("	==> server recved msg `%s`.\n", strMsg.c_str());
	}
	catch (const std::exception& Err)
	{
		printf("[ERROR] WINAPI:recv() |   `%s`, ErrorCode: `%d`\n", Err.what(), WSAGetLastError());
	}
}

void CConnSock::m_send(SOCKET SendSock)
{
	try
	{
		std::string strMsg = "Hello client??";
		int nLength = strMsg.length();
		int nSendRes = 0;
		nSendRes = ::send(SendSock, (const char*)&nLength, sizeof(nLength), 0);
		if (nSendRes == SOCKET_ERROR)
			throw std::exception("send length failure.");

		nSendRes = ::send(SendSock, strMsg.c_str(), strMsg.length(), 0);
		if (nSendRes == SOCKET_ERROR)
			throw std::exception("send data failure.");

		printf("[INFO] Successfully send msg to client.\n");

	}
	catch (const std::exception& Err)
	{
		printf("[ERROR] WINAPI:send() |   `%s`, ErrorCode: `%d`\n", Err.what(), WSAGetLastError());
	}
}

void CConnSock::closeSock(SOCKET CloseSock)
{
	SOCKET Close = (SOCKET)CloseSock;
	if (INVALID_SOCKET == Close)
	{
		int nErrorCode = ::WSAGetLastError();
		printf("[ERROR] WINAPI:closesocket() |  `Socket close failure`, ErrorCode: `%d` \n", nErrorCode);
	}
	closesocket(CloseSock);
	printf("[INFO] Connection closed\n\n");

}
