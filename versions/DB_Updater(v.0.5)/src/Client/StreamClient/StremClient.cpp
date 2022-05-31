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
	//closesocket(m_ClientSocket);
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
	
	char fName[256];
	ZeroMemory(fName, 256);
	Recv(fName, sizeof(fName)); // *1 파일 이름 받기

	FILE* fp = 0;
	fopen_s(&fp, fName, "wb");
	

	Recv(fBuf, PKT); // *2 파일 사이즈 받기
	long fSize = atol(fBuf);

	int totalBufNum = fSize / PKT + 1; // totalNum이 파일의 전체 사이즈임.
	printf("[INFO] File size value recieved from Server  : %d\n", totalBufNum);

	printf(" * * * \n");
	while (BufNum != totalBufNum) 
	{
		readBytes = Recv(fBuf, sizeof(fBuf)); // *3 파일 전체 받아오기
		BufNum++;

		fwrite(fBuf, sizeof(char), readBytes, fp);
		printf("[DOWNLOAD] In progress : %d / %dByte(s) [%d%%]\n", totalBufNum, fSize, ((BufNum * 100) / totalBufNum));
	}
	printf(" * * * \n");
	fclose(fp);


	return 0;
}




