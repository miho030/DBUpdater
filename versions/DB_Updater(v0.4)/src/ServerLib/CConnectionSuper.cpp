#include "stdafx.h"
#include "CConnectionSuper.h"

#define PACKET_SIZE 1024

void CConnectionSuper::ConnectionThread()
{
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* server)
{
	return 0;
}

int CConnectionSuper::Send(const LPBYTE pData, size_t tSize)
{

	int BufNum = 0;
	int readBytes;
	char fBuf[PACKET_SIZE];

	FILE* fp;
	fopen_s(&fp, "./file/file.png", "wb");
	recv(m_Socket, fBuf, PACKET_SIZE, 0);
	long fSize = atol(fBuf);
	int totalNum = fSize / PACKET_SIZE +1;

	while (BufNum != totalNum)
	{
		readBytes = recv(m_Socket, fBuf, PACKET_SIZE, 0);
		BufNum++;
		fwrite(fBuf, sizeof(char), readBytes, fp);
	}
	fclose(fp);

	return 0;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	int sendByte;
	long fSize;
	char fBuf[PACKET_SIZE];

	FILE *fp;
	fopen_s(&fp, "./file/file.png", "rb");
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	snprintf(fBuf, sizeof(fBuf), "%d", fSize);
	
	send(m_Socket, fBuf, sizeof(fBuf), 0);
	while ((sendByte = fread(fBuf, sizeof(char), sizeof(fBuf), fp)) > 0)
	{
		send(m_Socket, fBuf, sendByte, 0);
	}	
	fclose(fp);
	
	return 0;
}