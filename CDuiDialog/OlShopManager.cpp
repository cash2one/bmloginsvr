#include "OlShopManager.h"
//////////////////////////////////////////////////////////////////////////
OlShopManager::OlShopManager()
{
	m_nQuerySeed = 0;
}

OlShopManager::~OlShopManager()
{

}

//////////////////////////////////////////////////////////////////////////
bool OlShopManager::IsWaitForQuery(int _nUid)
{
	OlShopBuyQueryInfoMap::const_iterator fndIter = m_xOlShopBuyQueryInfoMap.find(_nUid);
	if(fndIter == m_xOlShopBuyQueryInfoMap.end())
	{
		return false;
	}
	return true;
}

int OlShopManager::SetWaitForQuery(int _nUid, bool _bSet)
{
	int nQueryID = GetQueryID(_nUid);
	if(!_bSet)
	{
		if(-1 != nQueryID)
		{
			//	已在等待，取消等待
			OlShopBuyQueryInfoMap::const_iterator fndIter = m_xOlShopBuyQueryInfoMap.find(_nUid);
			if(fndIter != m_xOlShopBuyQueryInfoMap.end())
			{
				m_xOlShopBuyQueryInfoMap.erase(fndIter);
				return 0;
			}
		}
	}
	else
	{
		//	设置等待
		if(-1 == nQueryID)
		{
			OlShopBuyQueryInfo info;
			info.nQueryID = ++m_nQuerySeed;
			m_xOlShopBuyQueryInfoMap.insert(std::make_pair(_nUid, info));
			return info.nQueryID;
		}
	}
}

int OlShopManager::GetQueryID(int _nUid)
{
	OlShopBuyQueryInfoMap::const_iterator fndIter = m_xOlShopBuyQueryInfoMap.find(_nUid);
	if(fndIter == m_xOlShopBuyQueryInfoMap.end())
	{
		return -1;
	}
	return fndIter->second.nQueryID;
}