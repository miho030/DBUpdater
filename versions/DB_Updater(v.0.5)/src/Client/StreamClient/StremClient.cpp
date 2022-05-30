#include "stdafx.h"
#include "StremClient.h"

#define PKT 1024
#define SAVE_DIR "./recvdDir/"
#define SAVE_FILE "./recvdDir/Img.jpeg"

/*  Class Creator & Destructor  */
CStreamClient::CStreamClient(void)
{
}
CStreamClient::~CStreamClient(void)
{
}



/*  Connection Functions  */
int CStreamClient::ConnectServer(const char* IPaddr, unsigned short wport)
{
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in addr;
	addr.sin_port = htons(wport);
	addr.sin_addr.s_addr = inet_addr(IPaddr);
	addr.sin_family = AF_INET;

	return connect(m_ClientSocket, (sockaddr*)&addr, sizeof(addr));
}

void CStreamClient::CloseConnection(void)
{
	closesocket(m_ClientSocket);
	WSACleanup();
	return;
}



/*  Data Sender & Reciever Functions  */
int CStreamClient::Send(const char* sData, int sSize)
{
	return ::send(m_ClientSocket, sData, sSize, 0);
}

int CStreamClient::Recv(char* pBuf, int pSize)
{
	return recv(m_ClientSocket, pBuf, pSize, 0);
}

int CStreamClient::FILEnDirExistChecker(const char* fileDir)
{
	try
	{
		if (_access(fileDir, 0) == -1)
		{
			if (1 != CreateDirectoryA(fileDir, NULL))
			{
				throw std::exception("Create download directory failure.");
			}
			printf("[FILE/INFO] Successfully create download folder.\n");
			return 0;
			
			/*
			if (-1 != GetFileAttributesA(fileName))
			{
				printf("[FILE/INFO] file already exist.\n");
				return 1;
			}
			*/
		}
	}
	catch (std::exception& ErrMsg)
	{
		printf("[ERROR] FILE/IO  | `%s`\n", ErrMsg.what());
	}
}


int CStreamClient::PreLoader(char* fNameBuf)
{
	char* NameBuffer = fNameBuf;
	/*  ���� �̸� �ޱ�  */
	int t_fNSize = 0;

	Recv((char*)t_fNSize, (int)sizeof(t_fNSize));
	Recv(NameBuffer, t_fNSize);

	printf("file name : %s\n", NameBuffer);
	printf("file name size : %d\n", t_fNSize);
	
	return t_fNSize;
}


int CStreamClient::Downloader()
{
	char* t_fNameBuf = 0;
	size_t BufNum = 0;
	char fBuf[PKT];

	/*  ���� �̸�, ũ�� �޾ƿ���  */
	if (int fNSize = PreLoader(t_fNameBuf) == NULL)
	{
		printf("[ERROR] File name recved failure.\n");
		return -1;
	}

	if (1 == FILEnDirExistChecker(SAVE_DIR))
	{  return 1; }

	//const char* savedName = SAVE_DIR + t_fNameBuf.c_str();


	/*  �����κ��� ���� ���ϸ����� ���� ����  */
	FILE* fp = 0;
	fopen_s(&fp, t_fNameBuf, "wb");

	int checkNum = 1;
	Send((const char*)checkNum, sizeof(checkNum));


	Recv(fBuf, PKT); // �����κ��� ���� ���� ����� fBuf�� �����
	long fSize = atol(fBuf); // char -> long ����ȯ

	int totalBufNum = fSize / PKT + 1; // totalNum�� ������ ��ü ��������.
	printf("[INFO] File size value recieved from Server  : %d\n", totalBufNum);


	printf(" * * * \n");
	while (BufNum !=totalBufNum)
	{
		int readBytes = Recv(fBuf, sizeof(fBuf));
		BufNum++;

		fwrite(fBuf, sizeof(char), readBytes, fp);
		printf("[DOWNLOAD] In progress : %d / %dByte(s) [%d%%]\n", totalBufNum, fSize, ((BufNum * 100) / totalBufNum));
	}
	printf(" * * * \n");

	if (fp == 0)
	{
		printf("[FILE/ERROR] file write error.\n");
	}
	fclose(fp);
	return 0;
}




