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
	printf("[INFO/File]  Successfully Connected to Client.\n");
}

void CStreamConnection::OnRecv()
{
	size_t fSize = 0;
	char fBuf[PKT];
	int BufNum = 0;
	int totalSendBytes = 0;

	const char* fDir = "./Sample/Img.jpeg";

	FILE* fp = 0;
	fopen_s(&fp, fDir, "rb");
	if (fp == NULL)
	{
		printf("[ERROR] File not exist.\n");
		exit(0);
	}

	/*  file size  */
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	int totalBufNum = fSize / sizeof(fBuf) + 1;
	fseek(fp, 0, SEEK_SET); // fBuf에 파일 사이즈 값 저장됨.
	

	snprintf(fBuf, sizeof(fBuf), "%d", fSize);
	printf("[INFO] file size value (fseek)  : %d\n", fSize);
	
	int SendBytes = Send(fBuf, sizeof(fBuf));
	if (SendBytes != 0)
	{
		printf("[INFO] Successfully send file size to client.\n");
	}

	

	/*  file transfer  */
	while ((SendBytes = fread(fBuf, sizeof(char), sizeof(fBuf), fp)) > 0)
	{

		Send(fBuf, SendBytes);
		BufNum++;
		totalSendBytes += SendBytes;
		//system("cls");
		printf("[INFO] In progress : %d / %dByte(s) [%d%%]\n", totalSendBytes, fSize, ((BufNum * 100) / totalBufNum));
	}
	fclose(fp);


	



	
	/*
	char fBuf[PKT], fSize[MAX_FILE_SIZE];
	int fileSize, recvdSize;


	while (true)
	{
		try
		{
			if (WSAGetLastError())
			{  throw std::exception("Can not connect to server.");  }


			ZeroMemory(fBuf, PKT);
			Recv(fBuf, PKT);


			fopen_s(&fp, fBuf, "rb");

			if (fp == 0)
			{
				throw std::exception("File buf is zero.");
			}


			fseek(fp, 0, SEEK_END);
			fileSize = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			ZeroMemory(fBuf, PKT);
			sprintf_s(fBuf, "%d", fileSize);

			Send(fBuf, PKT);

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


	}
	*/
	
}


/*  Close socket & WSA clean  */
void CStreamConnection::OnClose()
{
	closesocket(m_FileServerSocket);
	WSACleanup();

	printf("[INFO/File]  Successfully disconnected from client.\n");
}