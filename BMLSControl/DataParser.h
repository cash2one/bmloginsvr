#ifndef _INC_DATAPARSER_
#define _INC_DATAPARSER_
/************************************************************************/
/* DataParser 客户端socket数据接收解析类
/************************************************************************/
//////////////////////////////////////////////////////////////////////////
#include "ByteBuffer.h"
//#include "GamePacket.h"
#include <windows.h>
//////////////////////////////////////////////////////////////////////////
#define MAX_PACKET_SIZE		(4*1024)

#define PACKET_LENGTH_NETLONG

#ifdef PACKET_LENGTH_NETLONG
#define PACKET_LENGTH(L)	::ntohl(L)
#else
#define PACKET_LENGTH(L)	L
#endif
//////////////////////////////////////////////////////////////////////////
typedef void (__stdcall *HandlePacket)(const void*, unsigned int);
//////////////////////////////////////////////////////////////////////////
class DataParser
{
public:
	explicit DataParser(unsigned int _size = MAX_PACKET_SIZE);
	~DataParser();

public:
	void OnSocketData(const void* _pData, unsigned int _len);
	HandlePacket SetHandlePacket(HandlePacket _proc);

protected:
	ByteBuffer m_xBuffer;
	HandlePacket m_pHandler;
};

//////////////////////////////////////////////////////////////////////////
#endif