#include "CFileData.h"
#include "Shlwapi.h"

#include <list>
#include <iostream>
#include <fstream>
#include <iomanip>

//struct-functor for comparing to CFileData by short title
struct compareTitles
{
	bool operator()(FileData* first, FileData* second)
	{
		UINT i = 0;

		std::wstring firstName = first->m_shortFileName;
		std::wstring secondName = second->m_shortFileName;

		while ((i < firstName.length()) && (i < secondName.length()))
		{
			if (tolower(firstName[i])<tolower(secondName[i]))
			{
				return true;
			}
			else if (tolower(firstName[i])>tolower(secondName[i]))
			{
				return false;
			}

			++i;
		}

		return (firstName.length() < secondName.length());
	}
};

CFileData::CFileData()
{

}

CFileData::~CFileData()
{
	m_allElements.clear();
}

void CFileData::addNewFile(const wchar_t* dataMessage)
{
	tempFileData = new FileData();

	// save copy of string
	wcscpy_s(tempFileData->m_fullFileName, dataMessage); 
	
	//cut short file name from path
	std::wstring* a_FileName = new std::wstring[1];
	for (UINT i = 0; i < MAX_PATH; ++i)
	{
		a_FileName[0] += dataMessage[i];
	}
	wcscpy_s(tempFileData->m_shortFileName, PathFindFileName(a_FileName[0].c_str()));

	WIN32_FILE_ATTRIBUTE_DATA a_fileData;
	GetFileAttributesEx(dataMessage, GetFileExInfoStandard, &a_fileData);
	
	//get date of creation:
	FILETIME cr_time = a_fileData.ftCreationTime;
	FileTimeToSystemTime(&cr_time, &tempFileData->m_dataCreation);
	
	//get size of input file
	tempFileData->m_sizeFile = a_fileData.nFileSizeLow;

	m_allElements.push_back(tempFileData);
}

void CFileData::printInLog()
{
	std::wfstream file;

	file.open(LOG_NAME, std::ios::out);

	if (file)
	{
		for (std::list<FileData *>::iterator it = m_allElements.begin(); it != m_allElements.end(); ++it)
		{
			file << (*it)->m_shortFileName << std::setw(40 - wcslen((*it)->m_shortFileName)) <<
				(*it)->m_sizeFile << " byte" <<
				"    " << (*it)->m_dataCreation.wHour << ":" << (*it)->m_dataCreation.wMinute <<
				" " << (*it)->m_dataCreation.wDay << "." << (*it)->m_dataCreation.wMonth << "." << 
				(*it)->m_dataCreation.wYear << "\n";

		}
	}

	file.close();
}

void CFileData::sortByTitle()
{
	compareTitles functor;
	m_allElements.sort(functor);
}