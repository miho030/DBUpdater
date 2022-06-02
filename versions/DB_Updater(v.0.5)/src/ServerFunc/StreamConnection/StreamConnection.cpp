#include "../stdafx.h"
#include "StreamConnection.h"

#define PKT 1024

CStreamConnection::CStreamConnection(void)
{
}

CStreamConnection::~CStreamConnection(void)
{
}








/*   Server Util Functions  -> { GetFileName(), TimsStamp() }  */
const char* CStreamConnection::GetFileName(const char* file_path)
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

void CStreamConnection::TimeStamp()
{
	/*  현재 시간 구해서 string 값으로 변환  */
	time_t CurrentTime = time(NULL);
	struct tm st_TimeStamp;
	localtime_s(&st_TimeStamp, &CurrentTime);

	/*
	std::string year = std::to_string(st_TimeStamp.tm_year + 1900);
	std::string mon = std::to_string(st_TimeStamp.tm_mon + 1);
	std::string day = std::to_string(st_TimeStamp.tm_mon + 1);

	std::string hour = std::to_string(st_TimeStamp.tm_hour);
	std::string min = std::to_string(st_TimeStamp.tm_min);
	std::string sec = std::to_string(st_TimeStamp.tm_sec);

	printf(" *  %d.%d.%d  |  %d:%d:%d  *\n",
		st_TimeStamp.tm_year + 1900, st_TimeStamp.tm_mon + 1,
		st_TimeStamp.tm_mday, st_TimeStamp.tm_hour,
		st_TimeStamp.tm_min, st_TimeStamp.tm_sec);
	*/


	tm_Date =
		std::to_string(st_TimeStamp.tm_year + 1900) + "." + 
		std::to_string(st_TimeStamp.tm_mon + 1) + "." + 
		std::to_string(st_TimeStamp.tm_mon + 1);

	tm_Time =
		std::to_string(st_TimeStamp.tm_hour) + ":" +
		std::to_string(st_TimeStamp.tm_min) + ":" +
		std::to_string(st_TimeStamp.tm_sec);
}



/*   Server Main Functions  ->  { OnConnection(), OnRecv(), OnClose() }  */
void CStreamConnection::OnConnection()
{
	printf("[INFO/File]  Successfully Connected to Client.\n");
}


void CStreamConnection::OnRecv()
{
	int fSize = 0;
	char fBuf[PKT];
	int BufNum = 0;
	int totalSendBytes = 0;

	/*  보낼 파일 지정하고 파일 이름/이름의 크기 구하기  */
	const char* fDir = "./Sample/20220531.csv";
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
		_tprintf(TEXT("[ERROR] File not exist.\n"));
		exit(0);
	}


	/*  Stored File size value into 'fBuf'  */
	fseek(fp, 0, SEEK_END);
	fSize = ftell(fp);
	int totalBufNum = fSize / sizeof(fBuf) + 1;
	fseek(fp, 0, SEEK_SET); // 
	

	snprintf(fBuf, sizeof(fBuf), "%d", fSize);
	_tprintf(TEXT("[INFO] file size value (fseek)  : %d\n"), fSize);
	
	/*  Send to file size to cleint  */
	int SendBytes = Send(fBuf, sizeof(fBuf));
	if (SendBytes != 0)
	{
		_tprintf(TEXT("[INFO] Successfully send file size to client.\n"));
	}

	
	_tprintf(TEXT(" * * * \n"));
	/*  Send file to client  */
	while ((SendBytes = fread(fBuf, sizeof(char), sizeof(fBuf), fp)) > 0)
	{
		Send(fBuf, SendBytes); // *3 파일 전체 보내기
		BufNum++;
		totalSendBytes += SendBytes;

		/*  Pretty CLI Start  */
		Sleep(3);
		system("cls");
		printf(" - - - \n\n");
		
		_tprintf(TEXT("[FILE/SEND] In progress : %d / %dByte(s) [%d%%]\n"),
			totalSendBytes, fSize, ((BufNum * 100) / totalBufNum));
		
		printf(" - - - \n\n");
		/*  Pretty CLI End  */
	}
	fclose(fp);
	

	TimeStamp();
	
	_tprintf(TEXT("[INFO] Successfully send file to client.\n\n"));
	printf("     - File name : %s\n      - File size : %d\n     - Time stamp : %s | %s\n\n",
		fName, fSize, tm_Date.c_str(), tm_Time.c_str());
	_tprintf(TEXT("\n * * * \n\n"));
	
	return;
}


void CStreamConnection::OnClose()
{
	//closesocket(m_FileServerSocket);
	WSACleanup();
	_tprintf(TEXT("[INFO/File]  Successfully disconnected from client.\n\n"));
	_tprintf(TEXT("[INFO] Waiting for new client connection ...\n"));
	return;
}