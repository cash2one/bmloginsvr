#include "HideAttribHelper.h"
//////////////////////////////////////////////////////////////////////////
/*
	0000 0000 | 0000 0000 | 0000 0000 | 0000 0000
	2 bit:all attrib count
	2 bit:active attrib count
	4 bit:attrib type
	4 bit:attrib value
	...
*/
//////////////////////////////////////////////////////////////////////////
void HideAttribHelper::SetAllAttribCount(unsigned int& _refData, int _nCount)
{
	if(_nCount < 0 ||
		_nCount > 3){
			return;
	}

	unsigned int uMask = _nCount;
	uMask = uMask << 30;
	_refData |= uMask;
}

void HideAttribHelper::SetActiveAttribCount(unsigned int& _refData, int _nCount)
{
	if(_nCount < 0 ||
		_nCount > 3){
			return;
	}

	unsigned int uMask = _nCount;
	uMask = uMask << 28;
	_refData |= uMask;
}

void HideAttribHelper::SetActiveAttribType(unsigned int& _refData, int _nIndex, int _nType)
{
	if(_nType < 0 ||
		_nType > 0xffff){
			return;
	}

	unsigned int uMask = _nType;
	uMask = uMask << (8 + 8 * _nIndex);
	_refData |= uMask;
}

void HideAttribHelper::SetActiveAttribValue(unsigned int& _refData, int _nIndex, int _nValue)
{
	if(_nValue < 0 ||
		_nValue > 0xffff){
			return;
	}

	unsigned int uMask = _nValue;
	uMask = uMask << (4 + 8 * _nIndex);
	_refData |= uMask;
}

int HideAttribHelper::GetAllAttribCount(unsigned int _uData)
{
	unsigned int uMask = 0xc0000000;
	int nValue = (_uData & uMask) >> 30;
	return nValue;
}

int HideAttribHelper::GetActiveAttribCount(unsigned int _uData)
{
	unsigned int uMask = 0x30000000;
	int nValue = (_uData & uMask) >> 28;
	return nValue;
}

int HideAttribHelper::GetActiveAttribType(int _nIndex, unsigned int _uData)
{
	int nAllCount = GetAllAttribCount(_uData);

	if(_nIndex < 0 ||
		_nIndex >= 3){
			return 0;
	}

	unsigned int uMask = 0xf00;
	uMask = uMask << (_nIndex * 8);

	int nValue = (_uData & uMask) >> (8 + _nIndex * 8);
	return nValue;
}

int HideAttribHelper::GetActiveAttribValue(int _nIndex, unsigned int _uData)
{
	int nAllCount = GetAllAttribCount(_uData);

	if(_nIndex < 0 ||
		_nIndex >= 3){
			return 0;
	}

	unsigned int uMask = 0xf0;
	uMask = uMask << (_nIndex * 8);

	int nValue = (_uData & uMask) >> (4 + _nIndex * 8);
	return nValue;
}