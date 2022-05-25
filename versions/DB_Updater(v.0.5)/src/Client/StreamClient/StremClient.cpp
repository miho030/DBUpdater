#include "stdafx.h"
#include "StremClient.h"

#define PKT 1024
#define MAX_FILE_SIZE 100000

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

	/*  file dexcription  */
	FILE* fp = 0;
	std::string reqfileName;
	char fBuf[PKT], fSize[MAX_FILE_SIZE], fname[PKT];


	/*  Request to server */
	while (true)
	{

		ZeroMemory(fname, PKT);
		ZeroMemory(fBuf, PKT);
		reqfileName.clear();

		std::cout << "Type to load filename by server path : ";
		std::getline(std::cin, reqfileName);
		fflush(stdin);

		std::cout << "Set the filename will save : ";
		std::getline(std::cin, reqfileName);
		std::cin >> fname;
		fflush(stdin);


		/* Send 'file direction' to server,  which is stored in server  */
		Send(reqfileName.c_str(), PKT);
		/*  Recieve file size from server  */
		Recv(fBuf, PKT);

		fopen_s(&fp, fname, "wb");
		int fileSize = atoi(fBuf);


		/*  recieve file stream data from server  */
		while (true)
		{
			try
			{
				if (WSAGetLastError())
				{  throw std::exception("Can not connect to server.");  }
			
				ZeroMemory(fSize, MAX_FILE_SIZE);
				ZeroMemory(fBuf, PKT);

				Recv(fBuf, MAX_FILE_SIZE);
				Recv(fSize, PKT);
				printf("[INFO] File has been recieved,  |\n");
				printf("	¤¤ file name : `%s`   |  file size : `%d`\n", fname, fSize);

				fileSize -= atoi(fBuf);
				if (fileSize <= 0)
				{  throw std::exception("File recieve failure, file size is `zero`.");  }
			}
			catch (std::exception& ErrMsg)
			{
				printf("[ERROR] Connection|Recieve  |  `%s`\n", ErrMsg.what());
				return -1;
			}
		}
		fclose(fp);
	}

	return 0;
}




