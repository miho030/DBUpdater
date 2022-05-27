#include "stdafx.h"
#include "StremClient.h"

#define PKT 1024

/*  Class Creator & Destructor  */
CStremClient::CStremClient(void)
{
}
CStremClient::~CStremClient(void)
{
}



/*  Connection Functions  */
int CStremClient::ConnectServer(const char* IPaddr, unsigned short wport)
{
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	addr.sin_port = htons(wport);
	addr.sin_addr.s_addr = inet_addr(IPaddr);
	addr.sin_family = AF_INET;

	return connect(m_ClientSocket, (sockaddr*)&addr, sizeof(addr));
}

void CStremClient::CloseConnection(void)
{
	closesocket(m_ClientSocket);
	WSACleanup();
	return;
}



/*  Data Sender & Reciever Functions  */
int CStremClient::Send(const char* sData, int sSize)
{
	return ::send(m_ClientSocket, sData, sSize, 0);
}

int CStremClient::Recv(char* pBuf, int pSize)
{
	return recv(m_ClientSocket, pBuf, pSize, 0);
}



int CStremClient::Downloader()
{
	int readBytes;
	size_t BufNum = 0;
	char fBuf[PKT];

	const char* recvdDir = "./recvdImg.jpeg";
	

	FILE* fp = 0;
	fopen_s(&fp, recvdDir, "wb");
	


	Recv(fBuf, PKT); // 서버로부터 받은 파일 사이즈가 fBuf에 저장됨
	long fSize = atol(fBuf); // char -> long 형변환

	int totalBufNum = fSize / PKT + 1; // totalNum이 파일의 전체 사이즈임.
	printf("[INFO] File size value recieved from Server  : %d\n", totalBufNum);


	while (BufNum !=totalBufNum)
	{
		readBytes = Recv(fBuf, sizeof(fBuf));
		BufNum++;

		fwrite(fBuf, sizeof(char), readBytes, fp);
		printf("[INFO] In progress : %d / %dByte(s) [%d%%]\n", readBytes, fSize, ((BufNum * 100) / totalBufNum));
	}
	fclose(fp);


	return 0;
}




