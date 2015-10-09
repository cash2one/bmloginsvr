#ifndef _INC_THREADRUNNER_
#define _INC_THREADRUNNER_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <process.h>
#include <exception>
#include "MacroHelper.h"
//////////////////////////////////////////////////////////////////////////
typedef unsigned char ThreadNumber;
//////////////////////////////////////////////////////////////////////////
#define RUNTHREAD_PREPARED_EVENT	"RunThread prepared event"
//////////////////////////////////////////////////////////////////////////
class ThreadRunner
{
public:
	enum THREAD_STATE
	{
		TS_STOP,
		TS_RUN,
		TS_PAUSE
	};

	enum THREAD_RESULT
	{
		TR_SETEVENTFAILED = 0xFFFF0000
	};

public:
	ThreadRunner()
	{
		m_eThreadState = TS_STOP;
		m_hPreparedEvt = NULL;
		m_hThread = NULL;
		m_uThreadID = 0;
		m_dwRunSleepTime = 1;

		m_uTerminate = m_uPause = 0;
		m_dwLastRunCostTime = 0;

		m_hPreparedEvt = CreateEvent(NULL,
			FALSE,//	Autoreset
			FALSE,
			RUNTHREAD_PREPARED_EVENT);

		if(m_hPreparedEvt == NULL)
		{
			throw std::exception("Can't create the thread event in [ThreadRunner]");
		}
	}
	virtual ~ThreadRunner()
	{
		Stop();

		CloseHandle(m_hPreparedEvt);

		while(1)
		{
			if(GetThreadState() == TS_STOP)
			{
				break;
			}
		}
	}

public:
	virtual bool Run()
	{
		if(GetThreadState() != TS_STOP)
		{
			return false;
		}

		m_hThread = (HANDLE)_beginthreadex(NULL,
			0,
			&ThreadRunner::ThreadProc,
			this,
			0,
			&m_uThreadID);

		if(NULL == m_hThread)
		{
			return false;
		}

		return true;
	}
	virtual unsigned int Thread_DoWork()
	{
		return 0;
	}
	virtual unsigned int Thread_Initialize()
	{
		return 0;
	}
	virtual unsigned int Thread_ProcessMessage(const MSG* _pMsg)
	{
		return 0;
	}

public:
	void Stop()
	{
		m_uTerminate = 1;
		//m_eThreadState = TS_STOP;
	}
	void Pause()
	{
		m_uPause = 1;
		//m_eThreadState = TS_PAUSE;
	}
	void Resume()
	{
		m_uPause = 0;
		//m_eThreadState = TS_RUN;
	}

	void PostRunnerMessage(const MSG* _pMsg)
	{
		if(GetThreadState() == TS_RUN)
		{
			PostThreadMessage(GetThreadID(), _pMsg->message, _pMsg->wParam, _pMsg->lParam);
		}
	}

public:
	//	setter and getter
	READWRITE_INTERFACE(m_hThread, ThreadHandle, HANDLE);
	READWRITE_INTERFACE(m_uThreadID, ThreadID, unsigned int);
	READWRITE_INTERFACE(m_dwRunSleepTime, RunSleepTime, DWORD);
	READ_INTERFACE(m_eThreadState, ThreadState, THREAD_STATE);
	READ_INTERFACE(m_hPreparedEvt, PreparedEvt, HANDLE);
	READ_INTERFACE(m_dwLastRunCostTime, LastRunCostTime, DWORD);

private:
	static unsigned int __stdcall ThreadProc(void* _pParam)
	{
		ThreadRunner* pThread = (ThreadRunner*)_pParam;
		unsigned int uRet = 0;

		//	Create the message loop
		MSG msg;
		PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
		ZeroMemory(&msg, sizeof(MSG));

		if(!SetEvent(pThread->m_hPreparedEvt))
		{
			uRet = TR_SETEVENTFAILED;
		}

		//	Initialize
		uRet = pThread->Thread_Initialize();
		DWORD dwCurTick = GetTickCount();

		//	a runloop
		while(0 == uRet)
		{
			//	Process thread message
			if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				uRet = pThread->Thread_ProcessMessage(&msg);
			}
			if(0 != uRet)
			{
				break;
			}

			//	Stop or pause this thread
			if(pThread->m_uPause)
			{
				pThread->m_eThreadState = TS_PAUSE;
				SleepEx(pThread->GetRunSleepTime(), TRUE);
				continue;
			}
			if(pThread->m_uTerminate)
			{
				break;
			}

			//	Do thread work
			pThread->m_eThreadState = TS_RUN;
			dwCurTick = GetTickCount();

			uRet = pThread->Thread_DoWork();
			if(0 != uRet)
			{
				break;
			}

			pThread->m_dwLastRunCostTime = GetTickCount() - dwCurTick;

			//	Into sleep
			SleepEx(pThread->GetRunSleepTime(), TRUE);
		}

		pThread->m_eThreadState = TS_STOP;
		return uRet;
	}

protected:
	THREAD_STATE m_eThreadState;
	HANDLE m_hPreparedEvt;
	ThreadNumber m_uTerminate;
	ThreadNumber m_uPause;

	HANDLE m_hThread;
	unsigned int m_uThreadID;

	DWORD m_dwRunSleepTime;
	DWORD m_dwLastRunCostTime;
};
//////////////////////////////////////////////////////////////////////////
#endif