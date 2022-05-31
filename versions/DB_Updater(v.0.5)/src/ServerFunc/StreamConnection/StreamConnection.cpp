#include "../stdafx.h"
#include "StreamConnection.h"


#define PKT 1024



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

const char* GetFileName(const char* file_path)
{
	/*  지정된 디렉터리 경로에서 파일 이름 구하기  */
	const char* file_name = 0;
	while (*file_path)
	{
		if (*file_path == '/' && (file_path + 1) != NULL)
			file_name = file_path + 1;
		file_path++;
	}
	return file_name;
}


void CStreamConnection::OnRecv()
{
	int fSize = 0;
	char fBuf[PKT];
	int BufNum = 0;
	int totalSendBytes = 0;

	/*  보낼 파일 지정하고 파일 이름/이름의 크기 구하기  */
	const char* fDir = "./Sample/Img.jpeg";
	const char* t_fName = GetFileName(fDir);

	char fName[256];
	ZeroMemory(fName, 256);
	sprintf_s(fName, t_fName);
	printf("[FILE] file name/Size is : %s / %d\n", fName, sizeof(fName));


	Send((const char*)fName, sizeof(fName)); // *1 파일 이름 보내기

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
	
	int SendBytes = Send(fBuf, sizeof(fBuf)); // *2 파일 사이즈 보내기
	if (SendBytes != 0)
	{
		printf("[INFO] Successfully send file size to client.\n");
	}

	
	printf(" * * * \n");
	/*  file transfer  */
	while ((SendBytes = fread(fBuf, sizeof(char), sizeof(fBuf), fp)) > 0)
	{

		Send(fBuf, SendBytes); // *3 파일 전체 보내기
		BufNum++;
		totalSendBytes += SendBytes;
		//system("cls");
		printf("[SEND] In progress : %d / %dByte(s) [%d%%]\n", totalSendBytes, fSize, ((BufNum * 100) / totalBufNum));
	}
	printf(" * * * \n");
	fclose(fp);
}


/*  Close socket & WSA clean  */
void CStreamConnection::OnClose()
{
	//closesocket(m_FileServerSocket);
	WSACleanup();
	printf("[INFO/File]  Successfully disconnected from client.\n");
}