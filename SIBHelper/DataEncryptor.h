#ifndef _INC_DATAENCRYPTOR_
#define _INC_DATAENCRYPTOR_
//////////////////////////////////////////////////////////////////////////
#define ENCRYPT_MULTI_PARAM		125
#define ENCRYPT_NEXTKEY_PARAM	49
#define ENCRYPT_ORIGIN_KEY		0x2a
#define ENCRYPT_DEFAULT_KEY		0xd2d8
//////////////////////////////////////////////////////////////////////////
unsigned int EncryptBuffer(int _nEncryptKey, char* _pBuffer, unsigned int _uBufferSize);
unsigned int DecryptBuffer(int _nDecryptKey, char* _pBuffer, unsigned int _uBufferSize);
unsigned int _EncryptFile(int _nEncryptKey, const char* _pszFileName);
unsigned int _DecryptFile(int _nDecryptKey, const char* _pszFileName);
//////////////////////////////////////////////////////////////////////////
class DataEncryptor
{
public:
	static unsigned int DoEncryptBuffer(char* _pBuffer, unsigned int _uBufferSize);
	static unsigned int DoDecryptBuffer(char* _pBuffer, unsigned int _uBufferSize);

	static unsigned int DoEncryptFile(const char* _pszFileName);
	static unsigned int DoDecryptFile(const char* _pszFileName);
};
//////////////////////////////////////////////////////////////////////////
#endif