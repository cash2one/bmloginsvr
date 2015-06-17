#ifndef _INC_HIDEATTRIBHELPER_
#define _INC_HIDEATTRIBHELPER_
//////////////////////////////////////////////////////////////////////////
class HideAttribHelper
{
public:
	static int GetAllAttribCount(unsigned int _uData);
	static int GetActiveAttribCount(unsigned int _uData);
	static int GetActiveAttribType(int _nIndex, unsigned int _uData);
	static int GetActiveAttribValue(int _nIndex, unsigned int _uData);

	static void SetAllAttribCount(unsigned int& _refData, int _nCount);
	static void SetActiveAttribCount(unsigned int& _refData, int _nCount);
	static void SetActiveAttribType(unsigned int& _refData, int _nIndex, int _nType);
	static void SetActiveAttribValue(unsigned int& _refData, int _nIndex, int _nValue);
};
//////////////////////////////////////////////////////////////////////////
#endif