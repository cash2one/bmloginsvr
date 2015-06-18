#ifndef _INC_LOGINEXTENDINFOPARSER_
#define _INC_LOGINEXTENDINFOPARSER_
//////////////////////////////////////////////////////////////////////////
#include <string>
using std::string;
#include <vector>
using std::vector;
//////////////////////////////////////////////////////////////////////////
class LoginExtendInfoParser
{
public:
	LoginExtendInfoParser(const char* _pszContent);

public:
	void SetContent(const char* _pszContent);
	bool Parse();

	int GetDonateMoney();
	int GetGiftCount();
	int GetGiftID(int _nIndex);

private:
	string m_xContent;
	int m_nDonateMoney;
	vector<int> m_xGiftsID;
};
//////////////////////////////////////////////////////////////////////////
#endif