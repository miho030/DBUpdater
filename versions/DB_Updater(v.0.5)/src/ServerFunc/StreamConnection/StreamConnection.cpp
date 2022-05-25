#include "../stdafx.h"
#include "StreamConnection.h"

#define PKT 1024
#define MAX_FILE_SIZE 100000


CStreamConnection::CStreamConnection(void)
{
}

CStreamConnection::~CStreamConnection(void)
{
}


void CStreamConnection::OnConnection()
{
	printf("[INFO/File]  Successfully recieved file information from client.\n");
}

void CStreamConnection::OnRecv()
{
	CConnectionSuper* conn;

	FILE* fp = 0;
	char fBuf[PKT], fSize[MAX_FILE_SIZE];
	int fileSize, recvdSize;


	while (true)
	{
		try
		{
			if (WSAGetLastError())
			{  throw std::exception("Can not connect to server.");  }


			ZeroMemory(fBuf, PKT);
			conn->Recv(fBuf, PKT);


			fopen_s(&fp, fBuf, "rb");


			fseek(fp, 0, SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			ZeroMemory(fBuf, PKT);
			sprintf_s(fBuf, "%d", fileSize);

			conn->Send(fBuf, PKT);

			while (true)
			{
				ZeroMemory(fBuf, PKT);
				ZeroMemory(fSize, MAX_FILE_SIZE);

				recvdSize = fread(fSize, sizeof(char), MAX_FILE_SIZE, fp);
				_itoa_s(recvdSize, fSize, recvdSize, 0);

				Send(fBuf, PKT);
				Send(fSize, recvdSize);

				printf("[INFO/FILE]  Sending file ...  | `%d` bytes", recvdSize);

				fileSize -= recvdSize;
				if (fileSize <= 0)
				{
					throw std::exception("Read File failure.");
				}
			}
			fclose(fp);
		}
		catch (std::exception& ErrMsg)
		{
			printf("[ERROR] Connection|Recieve  |  `%s`\n", ErrMsg.what());
			break;
		}


		/* test 
			printf("[INFO/File]  Successfully send file to client\n");
			printf("	¤¤ file path: %s  |  file size: %d\n", fBuf, fileSize);
		*/
	}
	return;
}


/*  Close socket & WSA clean  */
void CStreamConnection::OnClose()
{
	closesocket(m_FileServerSocket);
	WSACleanup();

	printf("[INFO/File]  Successfully disconnected from client.\n");
}