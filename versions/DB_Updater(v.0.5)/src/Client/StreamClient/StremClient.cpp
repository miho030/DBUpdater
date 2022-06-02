#include "stdafx.h"
#include "StremClient.h"

#define PKT 1024

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



/*  Close Connection && WSAData structure Clean  */
void CStremClient::CloseConnection(void)
{
	closesocket(m_ClientSocket);
	WSACleanup();
	
	_tprintf(TEXT("[INFO] Successfully Disconnected from Update Server.\n"));
	return;
}



	/*  Data Sender & Reciever Functions  */
int CStremClient::Send(const char* sData, int sSize)
{  return ::send(m_ClientSocket, sData, sSize, 0);  }

int CStremClient::Recv(char* pBuf, int pSize)
{  return recv(m_ClientSocket, pBuf, pSize, 0);  }




/*  TimeStamp Functiuon  */
void CStremClient::TimeStamp()
{
	time_t CurrentTime = time(NULL);
	struct tm st_TimeStamp;
	localtime_s(&st_TimeStamp, &CurrentTime);

	tm_Date =
		std::to_string(st_TimeStamp.tm_year + 1900) + "." + 
		std::to_string(st_TimeStamp.tm_mon + 1) + "." + 
		std::to_string(st_TimeStamp.tm_mon + 1);

	tm_Time =
		std::to_string(st_TimeStamp.tm_hour) + ":" +
		std::to_string(st_TimeStamp.tm_min) + ":" +
		std::to_string(st_TimeStamp.tm_sec);
}



/*  StreamConnection Functions  */
int CStremClient::Downloader()
{
	int readBytes;
	size_t BufNum = 0;
	char fBuf[PKT];

	char fName[256];
	ZeroMemory(fName, 256);
	Recv(fName, sizeof(fName));

	FILE* fp = 0;
	fopen_s(&fp, fName, "wb");

	Recv(fBuf, PKT);
	long fSize = atol(fBuf);

	/*  Total File Size, 'totalBufNum'  */
	int totalBufNum = fSize / PKT + 1; 
	printf("[INFO] File size value recieved from Server  : %d\n",
		totalBufNum);



	printf(" * * * \n");
	while (BufNum != totalBufNum) 
	{
		readBytes = Recv(fBuf, sizeof(fBuf));
		BufNum++;

		/*  Pretty CLI Start  */
		Sleep(3);
		system("cls");
		printf(" - - - \n\n");
		
		fwrite(fBuf, sizeof(char), readBytes, fp);
		printf("[FILE/DOWNLOAD] In progress : %d / %dByte(s) [%d%%]\n",
			totalBufNum, fSize, ((BufNum * 100) / totalBufNum));
		
		printf("\n - - - \n\n");
		/*  Pretty CLI End  */
	}
	fclose(fp);


	TimeStamp();

	_tprintf(TEXT("[INFO] Successfully downlaod file from server.\n\n"));
	printf("     - File Name : %s\n     - File Size : %d\n     - TimeStamp : %s | %s\n\n",
		fName, fSize, tm_Date.c_str(), tm_Time.c_str());
	_tprintf(TEXT(" * * * \n\n"));

	return 0;
}