#include "ZipDataReader.h"
//////////////////////////////////////////////////////////////////////////
int ReadZipData(const char* _pszZipFileName, const char* _pszSubFileName, BYTE** _ppReadBuf, size_t* _uLen, const char* _pszZipPsw)
{
	CZipArchive xZipFile;
	CZipFileHeader* pHeader = NULL;

	if(!xZipFile.Open(_pszZipFileName))
	{
		return READZIP_ZIPFILE_NOTFOUND;
	}

	ZIP_INDEX_TYPE zIndex = xZipFile.FindFile(_pszSubFileName);
	if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
	{
		xZipFile.Close();
		return READZIP_SUBFILE_NOTFOUND;
	}

	if(NULL != _pszZipPsw)
	{
		xZipFile.SetPassword(_pszZipPsw);
	}

	pHeader = xZipFile.GetFileInfo(zIndex);
	xZipFile.OpenFile(zIndex);
	*_ppReadBuf = new BYTE[pHeader->m_uUncomprSize];
	*_uLen = pHeader->m_uUncomprSize;
	xZipFile.ReadFile(*_ppReadBuf, pHeader->m_uUncomprSize);
	
	xZipFile.CloseFile();
	xZipFile.Close();

	return READZIP_OK;
}