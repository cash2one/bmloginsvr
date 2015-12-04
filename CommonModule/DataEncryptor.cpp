#include <windows.h>
#include "DataEncryptor.h"
#include "xxtea.h"
//////////////////////////////////////////////////////////////////////////
static char s_szXXTeaKey[] = {12,43,15,77,123,35,94,1,76,88,12,65,117,49,92,10,25,78,26,74,18};
//////////////////////////////////////////////////////////////////////////
unsigned int EncryptBuffer(int _nEncryptKey, char* _pBuffer, unsigned int _uBufferSize)
{
	unsigned int uEncryptBytes = 0;

	if(NULL == _pBuffer ||
		0 == _uBufferSize)
	{
		return uEncryptBytes;
	}

	int nKey = _nEncryptKey | (int)_uBufferSize;
	nKey *= ENCRYPT_MULTI_PARAM;
	nKey = nKey >> 2;
	BYTE bKey = (BYTE)nKey;

	for(int i = 0;
		i < _uBufferSize;
		++i)
	{
		_pBuffer[i] = _pBuffer[i] ^ bKey;
		++uEncryptBytes;

		if(uEncryptBytes % 6 == 0)
		{
			BYTE bNextKey = bKey * ENCRYPT_NEXTKEY_PARAM + 5;

			if(bNextKey == 0)
			{
				//	255
				bNextKey = ENCRYPT_ORIGIN_KEY;
			}

			bKey = bNextKey;
		}
	}

	return uEncryptBytes;
}

unsigned int DecryptBuffer(int _nDecryptKey, char* _pBuffer, unsigned int _uBufferSize)
{
	unsigned int uEncryptBytes = 0;

	if(NULL == _pBuffer ||
		0 == _uBufferSize)
	{
		return uEncryptBytes;
	}

	int nKey = _nDecryptKey | (int)_uBufferSize;
	nKey *= ENCRYPT_MULTI_PARAM;
	nKey = nKey >> 2;
	BYTE bKey = (BYTE)nKey;

	for(int i = 0;
		i < _uBufferSize;
		++i)
	{
		_pBuffer[i] = _pBuffer[i] ^ bKey;
		++uEncryptBytes;

		if(uEncryptBytes % 6 == 0)
		{
			BYTE bNextKey = bKey * ENCRYPT_NEXTKEY_PARAM + 5;

			if(bNextKey == 0)
			{
				//	255
				bNextKey = ENCRYPT_ORIGIN_KEY;
			}

			bKey = bNextKey;
		}
	}

	return uEncryptBytes;
}

unsigned int _EncryptFile(int _nEncryptKey, const char* _pszFileName)
{
	unsigned int uEncryptBytes = 0;

	HANDLE hFile = CreateFile(_pszFileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		return uEncryptBytes;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMapping = NULL;

	do 
	{
		hFileMapping = CreateFileMapping(hFile,
			NULL,
			PAGE_READWRITE,
			0, 0, NULL);
		if(NULL == hFileMapping)
		{
			break;
		}

		char* pBytes = (char*)MapViewOfFile(hFileMapping,
			FILE_MAP_WRITE,
			0, 0, 0);
		if(NULL == pBytes)
		{
			break;
		}

		uEncryptBytes = EncryptBuffer(_nEncryptKey, pBytes, dwFileSize);

		UnmapViewOfFile(pBytes);
	} while (0);

	CloseHandle(hFileMapping);
	hFileMapping = NULL;
	CloseHandle(hFile);
	hFile = NULL;

	return uEncryptBytes;
}

unsigned int _DecryptFile(int _nDecryptKey, const char* _pszFileName)
{
	unsigned int uDecryptBytes = 0;

	HANDLE hFile = CreateFile(_pszFileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		return uDecryptBytes;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMapping = NULL;

	do 
	{
		hFileMapping = CreateFileMapping(hFile,
			NULL,
			PAGE_READWRITE,
			0, 0, NULL);
		if(NULL == hFileMapping)
		{
			break;
		}

		char* pBytes = (char*)MapViewOfFile(hFileMapping,
			FILE_MAP_WRITE,
			0, 0, 0);
		if(NULL == pBytes)
		{
			break;
		}

		uDecryptBytes = DecryptBuffer(_nDecryptKey, pBytes, dwFileSize);

		UnmapViewOfFile(pBytes);
	} while (0);

	CloseHandle(hFileMapping);
	hFileMapping = NULL;
	CloseHandle(hFile);
	hFile = NULL;

	return uDecryptBytes;
}



//////////////////////////////////////////////////////////////////////////
unsigned int DataEncryptor::DoEncryptBuffer(char* _pBuffer, unsigned int _uBufferSize)
{
	return EncryptBuffer(ENCRYPT_DEFAULT_KEY, _pBuffer, _uBufferSize);
}

unsigned int DataEncryptor::DoDecryptBuffer(char* _pBuffer, unsigned int _uBufferSize)
{
	return DecryptBuffer(ENCRYPT_DEFAULT_KEY, _pBuffer, _uBufferSize);
}

unsigned int DataEncryptor::DoEncryptFile(const char* _pszFileName)
{
	return _EncryptFile(ENCRYPT_DEFAULT_KEY, _pszFileName);
}

unsigned int DataEncryptor::DoDecryptFile(const char* _pszFileName)
{
	return _DecryptFile(ENCRYPT_DEFAULT_KEY, _pszFileName);
}

bool DataEncryptor::EncryptFileXXTea(const char* _pszFileName)
{
	unsigned int uEncryptBytes = 0;

	HANDLE hFile = CreateFile(_pszFileName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		return uEncryptBytes;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMapping = NULL;
	unsigned char* pEncryptData = NULL;
	xxtea_long uEncryptDataLength = 0;

	do 
	{
		hFileMapping = CreateFileMapping(hFile,
			NULL,
			PAGE_READWRITE,
			0, 0, NULL);
		if(NULL == hFileMapping)
		{
			break;
		}

		char* pBytes = (char*)MapViewOfFile(hFileMapping,
			FILE_MAP_WRITE,
			0, 0, 0);
		if(NULL == pBytes)
		{
			break;
		}

		pEncryptData = xxtea_encrypt((unsigned char*)pBytes, dwFileSize, (unsigned char*)s_szXXTeaKey, strlen(s_szXXTeaKey), &uEncryptDataLength);

		UnmapViewOfFile(pBytes);
	} while (0);

	CloseHandle(hFileMapping);
	hFileMapping = NULL;
	CloseHandle(hFile);
	hFile = NULL;

	if(NULL == pEncryptData)
	{
		return false;
	}
	else
	{
		hFile = CreateFile(_pszFileName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		do 
		{
			if(INVALID_HANDLE_VALUE == hFile)
			{
				return false;
			}

			DWORD dwWrite = 0;
			if(!WriteFile(hFile, pEncryptData, uEncryptDataLength, &dwWrite, NULL))
			{
				break;
			}
		} while (false);

		if(INVALID_HANDLE_VALUE != hFile)
		{
			CloseHandle(hFile);
			hFile = NULL;
		}
		free(pEncryptData);
		pEncryptData = NULL;
	}

	return true;
}