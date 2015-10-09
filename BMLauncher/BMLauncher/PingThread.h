#ifndef _INC_PING_THREAD_
#define _INC_PING_THREAD_
//////////////////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include "ThreadRunner.h"
#include <list>
//////////////////////////////////////////////////////////////////////////
//	ping struct declaration
struct PingTask
{
	int nTaskID;
	char szDeskIP[20];
	ULONG ulInetIP;
};
typedef std::list<PingTask*> PingTaskList;

#define DEF_PACKET_SIZE 32
#define ECHO_REQUEST 8
#define ECHO_REPLY 0

#define DEF_PINGTIMEOUT	200

#pragma pack(push, 1)

struct IPHeader
{
	BYTE m_byVerHLen; //4位版本+4位首部长度
	BYTE m_byTOS; //服务类型
	USHORT m_usTotalLen; //总长度
	USHORT m_usID; //标识
	USHORT m_usFlagFragOffset; //3位标志+13位片偏移
	BYTE m_byTTL; //TTL
	BYTE m_byProtocol; //协议
	USHORT m_usHChecksum; //首部检验和
	ULONG m_ulSrcIP; //源IP地址
	ULONG m_ulDestIP; //目的IP地址
};

struct ICMPHeader
{ 
	BYTE m_byType; //类型
	BYTE m_byCode; //代码
	USHORT m_usChecksum; //检验和 
	USHORT m_usID; //标识符
	USHORT m_usSeq; //序号
	ULONG m_ulTimeStamp; //时间戳（非标准ICMP头部）
};

#pragma pack(pop)

struct PingReply
{
	USHORT m_usSeq;
	DWORD m_dwRoundTripTime;
	DWORD m_dwBytes;
	DWORD m_dwTTL;
};
//////////////////////////////////////////////////////////////////////////
DWORD GetTickCountCalibrate();
//////////////////////////////////////////////////////////////////////////
class PingThread : public ThreadRunner
{
public:
	PingThread();
	virtual ~PingThread();

public:
	virtual unsigned int Thread_DoWork();

public:
	bool Init();
	void UnInit();

	int AddTask(const char* _pszIP);
	bool RemoveTask(int _nTaskID);
	void ClearTask();

	DWORD GetPingTimeout()
	{
		return m_dwPingTimeout;
	}
	void SetPingTimeout(DWORD _dwTimeout)
	{
		m_dwPingTimeout = _dwTimeout;
	}

	void SetReceiverMsgID(DWORD _dwMsgID)
	{
		m_dwReceiverMsgID = _dwMsgID;
	}
	void SetReceiverHwnd(HWND _hReceiver)
	{
		m_hReceiverHwnd = _hReceiver;
	}

protected:
	void LockTaskList()
	{
		EnterCriticalSection(&m_stCriticalSection);
	}
	void UnlockTaskList()
	{
		LeaveCriticalSection(&m_stCriticalSection);
	}
	bool DoPingWork(const PingTask* _pTask);
	USHORT CalCheckSum(USHORT *pBuffer, int nSize);

protected:
	bool m_bInitOk;
	PingTaskList m_xPingTaskList;
	CRITICAL_SECTION m_stCriticalSection;
	int m_nTaskIDSeed;
	int m_nPingICMPSeed;
	//	ICMP buffer
	char* m_szICMPData;
	//	socket relative
	SOCKET m_socketICMP;
	WSAEVENT m_event;
	USHORT m_usCurrentProcID;

	//	settings
	DWORD m_dwPingTimeout;

	//	receiver
	DWORD m_dwReceiverMsgID;
	HWND m_hReceiverHwnd;
};
//////////////////////////////////////////////////////////////////////////
#endif