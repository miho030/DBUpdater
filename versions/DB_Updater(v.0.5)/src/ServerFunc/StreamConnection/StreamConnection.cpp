#include "../stdafx.h"
#include "StreamConnection.h"


#define PKT 1024
#define FILE_DIR "./Sample/Img.jpeg"


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


char* CStreamConnection::GetFileName(char* file_path)
{
	/*  지정된 디렉터리 경로에서 파일 이름 구하기  */
	char* file_name = 0;
	while (*file_path)
	{
		if (*file_path == '/' && (file_path + 1) != NULL)
			file_name = file_path + 1;
		file_path++;
	}
	return file_name;
}

void CStreamConnection::PreLoader()
{
	/*  보낼 파일 지정하고 파일 이름/이름의 크기 구하기  */
	const char* fName = GetFileName((char*)FILE_DIR);
	int fNameSize = sizeof(fName);
	printf("[FILE] file name/Size is : %s / %d\n", fName, fNameSize);

	Send((const char*)fNameSize, (int)sizeof(fNameSize));  // 파일 이름의 사이즈부터 보낸다..
	Send((char*)fName, (int)sizeof(fNameSize)); // 파일 이름 전송
}

void CStreamConnection::OnRecv()
{
	int fSize = 0;
	char fBuf[PKT];
	int BufNum = 0;
	int totalSendBytes = 0;

	const char* fDirptr = FILE_DIR;

	FILE* fp = 0;
	fopen_s(&fp, fDirptr, "rb");
	if (fp == NULL)
	{
		printf("[ERROR] File not exist. OR can not open the file.\n");
		exit(0);
	}

	/*  클라이언트에게 파일 이름 전송  */
	PreLoader();


	/*  file size  */
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	int totalBufNum = fSize / sizeof(fBuf) + 1;
	fseek(fp, 0, SEEK_SET); // fBuf에 파일 사이즈 값 저장됨.
	

	snprintf(fBuf, sizeof(fBuf), "%d", fSize);
	printf("[INFO] file size value (fseek)  : %d\n", fSize);
	
	int SendBytes = Send(fBuf, sizeof(fBuf));
	if (SendBytes != 0)
	{  printf("[INFO] Successfully send file size to client.\n");  }


	int nRet = 0;
	Recv((char*)nRet, sizeof(nRet));

	if (1 == nRet)
	{
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

		/*  FIN 패킷 처리  */
		char tmp;
		::recv(m_FileServerSocket, (char*)&tmp, (int)sizeof(tmp), 0);
	}

}


/*  Close socket & WSA clean  */
void CStreamConnection::OnClose()
{
	closesocket(m_FileServerSocket);
	WSACleanup();

	printf("[INFO/File]  Successfully disconnected from client.\n");
}