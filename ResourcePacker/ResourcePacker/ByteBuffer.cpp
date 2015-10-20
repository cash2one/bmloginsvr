#include "ByteBuffer.h"
#include <memory.h>
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* ByteBuffer
/************************************************************************/
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/* Read
/************************************************************************/
unsigned int ByteBuffer::Read(void* _pData, unsigned int _nLen)
{
	if(m_uCurPointer + _nLen > m_uDestPointer)
	{
		return 0;
	}

	if(_nLen == 0)
	{
		return 1;
	}

	memcpy(_pData, m_szBuffer + m_uCurPointer, _nLen);
	m_uCurPointer += _nLen;
	return _nLen;
}

/************************************************************************/
/* Write
/************************************************************************/
unsigned int ByteBuffer::Write(const void* _pData, unsigned int _nLen)
{
	//if(m_uDestPointer + _nLen > MAX_BUFFER_SIZE)
	if(m_uDestPointer + _nLen > m_uMaxBufferSize)
	{
		return 0;
	}

	if(_nLen == 0)
	{
		return 1;
	}

	memcpy(m_szBuffer + m_uDestPointer, _pData, _nLen);
	m_uDestPointer += _nLen;
	return _nLen;
}

/************************************************************************/
/* operator= and copy constructor(protected)
/************************************************************************/
ByteBuffer& ByteBuffer::operator=(const ByteBuffer& _xBuf)
{
	m_uDestPointer = _xBuf.m_uDestPointer;
	m_uCurPointer = _xBuf.m_uCurPointer;
	delete[] m_szBuffer;
	m_uMaxBufferSize = _xBuf.m_uMaxBufferSize;
	m_szBuffer = new unsigned char[m_uMaxBufferSize];
	memcpy(m_szBuffer, _xBuf.m_szBuffer, _xBuf.GetLength());
	return (*this);
}

ByteBuffer::ByteBuffer(const ByteBuffer& _xBuf)
{
	m_uDestPointer = _xBuf.m_uDestPointer;
	m_uCurPointer = _xBuf.m_uCurPointer;
	m_uMaxBufferSize = _xBuf.m_uMaxBufferSize;
	m_szBuffer = new unsigned char[m_uMaxBufferSize];
	memcpy(m_szBuffer, _xBuf.m_szBuffer, _xBuf.GetLength());
}

/************************************************************************/
/* std::string ToHexString()
/************************************************************************/
std::string ByteBuffer::ToHexString()
{
	std::string strRet;
	strRet.resize(GetLength() * 2);
	char szValue[3];

	for(int i = 0; i < GetLength(); ++i)
	{
		sprintf(szValue, "%02x", m_szBuffer[i]);
		strRet[2 * i] = szValue[0];
		strRet[2 * i + 1] = szValue[1];
	}

	return strRet;
}




//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* 1 2 4
/************************************************************************/
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, char& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(char)))
	{
		READ_EXCEPTION(char);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, char _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(char)))
	{
		WRITE_EXCEPTION(char);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, unsigned char& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(unsigned char)))
	{
		READ_EXCEPTION(unsigned char);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, unsigned char _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(unsigned char)))
	{
		WRITE_EXCEPTION(unsigned char);
	}
	return _xBuffer;
}

//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, short& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(short)))
	{
		READ_EXCEPTION(short);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, short _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(short)))
	{
		WRITE_EXCEPTION(short);
	}
	return _xBuffer;
}

PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, unsigned short& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(unsigned short)))
	{
		READ_EXCEPTION(unsigned short);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, unsigned short _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(unsigned short)))
	{
		WRITE_EXCEPTION(unsigned short);
	}
	return _xBuffer;
}

//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, int& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(int)))
	{
		READ_EXCEPTION(int);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, int _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(int)))
	{
		WRITE_EXCEPTION(int);
	}
	return _xBuffer;
}

PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, unsigned int& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(unsigned int)))
	{
		READ_EXCEPTION(unsigned int);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, unsigned int _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(unsigned int)))
	{
		WRITE_EXCEPTION(unsigned int);
	}
	return _xBuffer;
}


//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, float& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(float)))
	{
		READ_EXCEPTION(float);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, float _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(float)))
	{
		WRITE_EXCEPTION(float);
	}
	return _xBuffer;
}

PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, double& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(double)))
	{
		READ_EXCEPTION(double);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, double _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(double)))
	{
		WRITE_EXCEPTION(double);
	}
	return _xBuffer;
}


//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, bool& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(bool)))
	{
		READ_EXCEPTION(bool);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, bool _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(bool)))
	{
		WRITE_EXCEPTION(bool);
	}
	return _xBuffer;
}


//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, char* _var)
{
	size_t len = 0;
	_xBuffer >> len;

	if(len != 0)
	{
		if(0 == _xBuffer.Read(_var, len + 1))
		{
			READ_EXCEPTION(char*);
		}
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, const char* _var)
{
	size_t len = strlen(_var);
	_xBuffer << len;

	if(len != 0)
	{
		if(0 == _xBuffer.Write(_var, len + 1))
		{
			WRITE_EXCEPTION(const char*);
		}
	}
	
	return _xBuffer;
}


//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, std::string& _var)
{
	size_t uSize = 0;
	_xBuffer >> uSize;
	if(0 != uSize)
	{
		_var = (char*)_xBuffer.GetHead();
		_xBuffer.SetHeadOffset(uSize + 1);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, const std::string& _var)
{
	size_t uSize = _var.length();
	_xBuffer << uSize;
	if(uSize != 0)
	{
		if(0 == _xBuffer.Write(_var.c_str(), uSize + 1))
		{
			WRITE_EXCEPTION(string);
		}
	}
	return _xBuffer;
}


//////////////////////////////////////////////////////////////////////////
PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, long& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(long)))
	{
		READ_EXCEPTION(long);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, long _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(long)))
	{
		WRITE_EXCEPTION(long);
	}
	return _xBuffer;
}

PREFIX ByteBuffer& operator>>(ByteBuffer& _xBuffer, unsigned long& _var)
{
	if(0 == _xBuffer.Read(&_var, sizeof(unsigned long)))
	{
		READ_EXCEPTION(unsigned long);
	}
	return _xBuffer;
}
PREFIX ByteBuffer& operator<<(ByteBuffer& _xBuffer, unsigned long _var)
{
	if(0 == _xBuffer.Write(&_var, sizeof(unsigned long)))
	{
		WRITE_EXCEPTION(unsigned long);
	}
	return _xBuffer;
}



//////////////////////////////////////////////////////////////////////////
