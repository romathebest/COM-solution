#pragma once

#define LOG_NAME "C:/log.txt"

#include <windows.h>
#include <list>

struct FileData
{
	wchar_t m_fullFileName[MAX_PATH];
	wchar_t m_shortFileName[MAX_PATH];
	DWORD m_sizeFile;
	SYSTEMTIME m_dataCreation;
};

class CFileData
{
public:
	CFileData();
	~CFileData();

	void addNewFile(const wchar_t* dataMessage);
	void printInLog();
	void sortByTitle();

private:
	FileData *tempFileData;
	std::list<FileData *> m_allElements;
};