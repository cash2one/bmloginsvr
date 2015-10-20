#ifndef _INC_COMMANDLINEHELPER_
#define _INC_COMMANDLINEHELPER_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <string>
#include <list>
using std::string;
typedef std::list<string> AppParamList;
//////////////////////////////////////////////////////////////////////////
class CommandLineHelper
{
public:
	CommandLineHelper();
	~CommandLineHelper();

public:
	bool InitParam();
	void ReleaseParam();

	const char* GetParam(const char* _pszSection);

	inline char GetSplitterChar()			{return m_cSplitterChar;}
	inline void SetSplitterChar(char _cVar)	{m_cSplitterChar = _cVar;}

private:
	AppParamList m_xParamList;
	char m_cSplitterChar;
};
//////////////////////////////////////////////////////////////////////////
#endif