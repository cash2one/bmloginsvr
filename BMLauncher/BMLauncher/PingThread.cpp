#include "StdAfx.h"
#include "PingThread.h"
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Ws2_32.lib")
//////////////////////////////////////////////////////////////////////////
ULONG GetTickCountCalibrate()
{
	static ULONG s_ulFirstCallTick = 0;
	static LONGLONG s_ullFirstCallTickMS = 0;

	SYSTEMTIME systemtime;
	FILETIME filetime;
	GetLocalTime(&systemtime);    
	SystemTimeToFileTime(&systemtime, &filetime);
	LARGE_INTEGER liCurrentTime;
	liCurrentTime.HighPart = filetime.dwHighDateTime;
	liCurrentTime.LowPart = filetime.dwLowDateTime;
	LONGLONG llCurrentTimeMS = liCurrentTime.QuadPart / 10000;

	if (s_ulFirstCallTick == 0)
	{
		s_ulFirstCallTick = GetTickCount();
	}
	if (s_ullFirstCallTickMS == 0)
	{
		s_ullFirstCallTickMS = llCurrentTimeMS;
	}

	return s_ulFirstCallTick + (ULONG)(llCurrentTimeMS - s_ullFirstCallTickMS);
}
//////////////////////////////////////////////////////////////////////////
PingThread::PingThread()
{
	InitializeCriticalSection(&m_stCriticalSection);
	m_nTaskIDSeed = 0;
	m_nPingICMPSeed = 0;
	m_bInitOk = false;
	m_szICMPData = NULL;
	m_dwPingTimeout = DEF_PINGTIMEOUT;
	m_usCurrentProcID = 0;
	m_dwReceiverMsgID = 0;
	m_hReceiverHwnd = NULL;
}

PingThread::~PingThread()
{

}

//////////////////////////////////////////////////////////////////////////
bool PingThread::Init()
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(1, 1), &WSAData);
	m_event = WSACreateEvent();
	m_socketICMP = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0, 0);

	if (m_socketICMP != SOCKET_ERROR)
	{
		WSAEventSelect(m_socketICMP, m_event, FD_READ);
		m_szICMPData = new char[DEF_PACKET_SIZE + sizeof(ICMPHeader)];
		m_bInitOk = true;
	}
	else
	{
		int nErrorCode = WSAGetLastError();
		char szErrorMsg[MAX_PATH];
		sprintf(szErrorMsg, "Socket error:%d, 请以管理员身份运行此程序", nErrorCode);
		AfxMessageBox(szErrorMsg, MB_OK | MB_ICONERROR);
		UnInit();
	}

	return m_bInitOk;
}

void PingThread::UnInit()
{
	ClearTask();
	WSACleanup();
	delete[] m_szICMPData;
	m_szICMPData = NULL;
}

unsigned int PingThread::Thread_DoWork()
{
	LockTaskList();

	PingTaskList::iterator begIter = m_xPingTaskList.begin();
	PingTaskList::iterator endIter = m_xPingTaskList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		PingTask* pTask = *begIter;
		DoPingWork(pTask);
	}


	UnlockTaskList();

	return 0;
}

int PingThread::AddTask(const char* _pszIP)
{
	unsigned long ulAddr = inet_addr(_pszIP);
	if(ulAddr == INADDR_NONE)
	{
		return 0;
	}

	LockTaskList();

	//	检测是否已经有相同的任务了
	int nTaskExistsID = 0;
	PingTaskList::iterator begIter = m_xPingTaskList.begin();
	PingTaskList::iterator endIter = m_xPingTaskList.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		PingTask* pTask = *begIter;

		if(pTask->ulInetIP == ulAddr)
		{
			nTaskExistsID = pTask->nTaskID;
			break;
		}
	}

	if(0 == nTaskExistsID)
	{
		PingTask* pTask = new PingTask;
		strcpy(pTask->szDeskIP, _pszIP);
		pTask->ulInetIP = ulAddr;
		pTask->nTaskID = ++m_nTaskIDSeed;
		m_xPingTaskList.push_back(pTask);
		nTaskExistsID = pTask->nTaskID;
	}

	UnlockTaskList();

	return nTaskExistsID;
}

bool PingThread::RemoveTask(int _nTaskID)
{
	LockTaskList();

	bool bRet = false;
	PingTaskList::iterator begIter = m_xPingTaskList.begin();
	PingTaskList::iterator endIter = m_xPingTaskList.end();

	for(begIter;
		begIter != endIter;
		)
	{
		PingTask* pTask = *begIter;

		if(0 == _nTaskID)
		{
			//	remove all
			delete pTask;
			pTask = NULL;
			begIter = m_xPingTaskList.erase(begIter);
		}
		else
		{
			if(pTask->nTaskID == _nTaskID)
			{
				delete pTask;
				pTask = NULL;
				m_xPingTaskList.erase(begIter);
				bRet = true;
				break;
			}
			else
			{
				++begIter;
			}
		}
	}

	UnlockTaskList();

	if(0 == _nTaskID)
	{
		return true;
	}
	else
	{
		return bRet;
	}
}

void PingThread::ClearTask()
{
	RemoveTask(0);
}

bool PingThread::DoPingWork(const PingTask* _pTask)
{
	if(!m_bInitOk)
	{
		return false;
	}

	//配置SOCKET
	sockaddr_in sockaddrDest; 
	sockaddrDest.sin_family = AF_INET; 
	sockaddrDest.sin_addr.s_addr = _pTask->ulInetIP;
	int nSockaddrDestSize = sizeof(sockaddrDest);

	//构建ICMP包
	int nICMPDataSize = DEF_PACKET_SIZE + sizeof(ICMPHeader);
	ULONG ulSendTimestamp = GetTickCountCalibrate();
	USHORT usSeq = ++m_nPingICMPSeed;    
	memset(m_szICMPData, 0, nICMPDataSize);
	ICMPHeader *pICMPHeader = (ICMPHeader*)m_szICMPData;
	pICMPHeader->m_byType = ECHO_REQUEST; 
	pICMPHeader->m_byCode = 0; 
	pICMPHeader->m_usID = m_usCurrentProcID;    
	pICMPHeader->m_usSeq = usSeq;
	pICMPHeader->m_ulTimeStamp = ulSendTimestamp;
	pICMPHeader->m_usChecksum = CalCheckSum((USHORT*)m_szICMPData, nICMPDataSize);

	//发送ICMP报文
	if (sendto(m_socketICMP, m_szICMPData, nICMPDataSize, 0, (struct sockaddr*)&sockaddrDest, nSockaddrDestSize) == SOCKET_ERROR)
	{
		return false;
	}

	char recvbuf[256] = {"\0"};
	while (TRUE)
	{
		//接收响应报文
		if (WSAWaitForMultipleEvents(1, &m_event, FALSE, m_dwPingTimeout, FALSE) != WSA_WAIT_TIMEOUT)
		{
			WSANETWORKEVENTS netEvent;
			WSAEnumNetworkEvents(m_socketICMP, m_event, &netEvent);

			if (netEvent.lNetworkEvents & FD_READ)
			{
				ULONG nRecvTimestamp = GetTickCountCalibrate();
				int nPacketSize = recvfrom(m_socketICMP, recvbuf, 256, 0, (struct sockaddr*)&sockaddrDest, &nSockaddrDestSize);
				if (nPacketSize != SOCKET_ERROR)
				{
					IPHeader *pIPHeader = (IPHeader*)recvbuf;
					USHORT usIPHeaderLen = (USHORT)((pIPHeader->m_byVerHLen & 0x0f) * 4);
					ICMPHeader *pICMPHeader = (ICMPHeader*)(recvbuf + usIPHeaderLen);

					if (pICMPHeader->m_usID == m_usCurrentProcID //是当前进程发出的报文
						&& pICMPHeader->m_byType == ECHO_REPLY //是ICMP响应报文
						&& pICMPHeader->m_usSeq == usSeq //是本次请求报文的响应报文
						)
					{
						/*pPingReply->m_usSeq = usSeq;
						pPingReply->m_dwRoundTripTime = nRecvTimestamp - pICMPHeader->m_ulTimeStamp;
						pPingReply->m_dwBytes = nPacketSize - usIPHeaderLen - sizeof(ICMPHeader);
						pPingReply->m_dwTTL = pIPHeader->m_byTTL;
						return TRUE;*/
						if(0 != m_dwReceiverMsgID &&
							NULL != m_hReceiverHwnd)
						{
							PostMessage(m_hReceiverHwnd, m_dwReceiverMsgID, _pTask->ulInetIP, nRecvTimestamp - pICMPHeader->m_ulTimeStamp);
							return true;
						}
					}
				}
			}
		}
		//超时
		if (GetTickCountCalibrate() - ulSendTimestamp >= m_dwPingTimeout)
		{
			PostMessage(m_hReceiverHwnd, m_dwReceiverMsgID, _pTask->ulInetIP, -1);
			return false;
		}
	}
}

USHORT PingThread::CalCheckSum(USHORT *pBuffer, int nSize)
{
	unsigned long ulCheckSum=0; 
	while(nSize > 1) 
	{ 
		ulCheckSum += *pBuffer++; 
		nSize -= sizeof(USHORT); 
	}
	if(nSize ) 
	{ 
		ulCheckSum += *(UCHAR*)pBuffer; 
	} 

	ulCheckSum = (ulCheckSum >> 16) + (ulCheckSum & 0xffff); 
	ulCheckSum += (ulCheckSum >>16); 

	return (USHORT)(~ulCheckSum); 
}