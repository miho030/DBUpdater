#include "TestCodes.h"

#define DB_DIRECTORY "./Sample/report_Sample/"
#define BUFSIZE 512

BOOL GetFileNameFromHandle(HANDLE hFile)
{
	BOOL bSuccess = FALSE;
	TCHAR pszFilename[MAX_PATH + 1];
	HANDLE hFileMap;

	/*  Get file size  */
	DWORD dwFileSizeHi = 0;
	DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi);

	if (dwFileSizeLo == 0 && dwFileSizeHi == 0)
	{
		_tprintf(TEXT("[FILE/ERROR] Cannot map a file with a length of zero.\n"));
		return -1;
	}
	
	/*  Create file mapping object  */
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 1, NULL);

	if (hFileMap)
	{
		void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
		if (pMem)
		{
			if (GetMappedFileName(GetCurrentProcess(), pMem, pszFilename, MAX_PATH))
			{
				/*  Translate path with device name to drive letters.  */
				TCHAR szTemp[BUFSIZE];
				szTemp[0] = '\0';

				if (GetLogicalDriveStrings(BUFSIZE - 1, szTemp))
				{
					TCHAR szName[MAX_PATH];
					TCHAR szDrive[3] = TEXT(" :");
					BOOL bFound = FALSE;
					TCHAR* p = szTemp;

					do
					{
						/*  Copy the drive letter to the emplate string  */
						*szDrive = *p;
						
						if (QueryDosDevice(szDrive, szName, MAX_PATH))
						{
							size_t uNameLen = _tcslen(szName);

							if (uNameLen < MAX_PATH)
							{
								bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0
									&& *(pszFilename + uNameLen) == _T('\\');
							
								if (bFound)
								{
									/*  Reconstruct pszFilename using szTempFile,
									    Replace device path with DOS path  */
									TCHAR szTempFile[MAX_PATH];
									StringCchPrintf(szTempFile, MAX_PATH, TEXT("%s%s"), szDrive, pszFilename+uNameLen);
									StringCchCopyN(pszFilename, MAX_PATH + 1, szTempFile, _tcslen(szTempFile));
								}
							}
						}
						while (*p++);
					} while (!bFound && *p);
				}
			}
			bSuccess = TRUE;
			UnmapViewOfFile(pMem);
		}
		CloseHandle(hFileMap);
	}
	_tprintf(TEXT("[FILE/ERROR] CreateFile failed with %d\n"), GetLastError());

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




int _tmain(int argc, TCHAR *argv[])
{

	time_t CurrentTime = time(NULL);
	struct tm st_TimeStamp;
	localtime_s(&st_TimeStamp, &CurrentTime);
	
	printf(" *  %d.%d.%d  |  %d:%d:%d  *\n", 
		st_TimeStamp.tm_year + 1900, st_TimeStamp.tm_mon + 1,
		st_TimeStamp.tm_mday, st_TimeStamp.tm_hour,
		st_TimeStamp.tm_min, st_TimeStamp.tm_sec);

	HANDLE hFile;
	
	if (argc != 2)
	{
		_tprintf(TEXT("Thie Sample takes a file name as a prameter.\n"));
		return 0;
	}
	hFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("CreateFile failed,  with ErrorNum: %d\n"), GetLastError());
		return 0;
	}
	GetFileNameFromHandle(hFile);
}