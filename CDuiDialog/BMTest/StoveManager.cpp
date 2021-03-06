#include "StoveManager.h"
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <Shlwapi.h>
//#include "GDefine.h"
#include "SimpleIni.h"
//////////////////////////////////////////////////////////////////////////
#ifndef SAFE_DELETE_ARRAY
#define	SAFE_DELETE_ARRAY(p)	{if(p){delete[] p;p=NULL;}}
#endif

#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////////
using std::string;
using std::vector;
//////////////////////////////////////////////////////////////////////////
int split(const string& str, vector<string>& ret_, string sep = ",")
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
StoveManager::StoveManager()
{
	ZeroMemory(m_ayLifeSkills, sizeof(m_ayLifeSkills));
}

StoveManager::~StoveManager()
{

}

bool StoveManager::Init()
{
	char szFile[MAX_PATH];
	GetModuleFileName(NULL, szFile, sizeof(szFile) - 1);
	PathRemoveFileSpec(szFile);

	char* pszFileData = NULL;

#ifdef _DEBUG
	strcat(szFile, "\\Help\\stove.ini");
#else
	strcat(szFile, "\\Help\\mouse.idx");
#endif

	if(!PathFileExists(szFile))
	{
		return false;
	}

	size_t uDataSize = 0;

#ifdef _DEBUG
	HANDLE hFile = ::CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}
	size_t uFileSize = GetFileSize(hFile, NULL);

	if(0 != uFileSize)
	{
		uDataSize = uFileSize;
		pszFileData = new char[uDataSize];
		pszFileData[uDataSize - 1] = 0;
	}
	DWORD dwRead = 0;
	if(FALSE == ReadFile(hFile, pszFileData, uFileSize, &dwRead, NULL))
	{
		CloseHandle(hFile);
		hFile = 0;
		delete[] pszFileData;
		pszFileData = NULL;
	}
	CloseHandle(hFile);
#else
	if(READZIP_OK == ReadZipData(szFile, "stove.ini", (BYTE**)&pszFileData, &uDataSize, SaveFile::CalcInternalPassword()))
	{
		
	}
#endif

	bool bRet = false;

	if(pszFileData != NULL)
	{
#ifdef NDEBUG
		//	decrypt it
		DataEncryptor::DoDecryptBuffer(pszFileData, uDataSize);
#endif
		bRet = LoadBuffer(pszFileData, uDataSize);
		SAFE_DELETE_ARRAY(pszFileData);
	}

	return bRet;
}

bool StoveManager::LoadBuffer(const char* _pBuffer, size_t _uSize)
{
	CSimpleIniA xIniFile;
	SI_Error eError = xIniFile.LoadData(_pBuffer, _uSize);

	if(SI_OK != eError)
	{
		return false;
	}

	//	load life skill
	char szKey[5] = {0};
	for(int i = kLifeSkill_SmeltOre; i < kLifeSkill_Total; ++i)
	{
		const char* pszDef = xIniFile.GetValue("LIFESKILLDEF", itoa(i, szKey, 10), "");

		if(0 == pszDef)
		{
			assert("Invalid life skill definition." == NULL);
			return false;
		}

		string xWaitSplit = pszDef;
		vector<string> xSections;
		split(xWaitSplit, xSections, "|");

		if(0 == xSections.size() ||
			xSections.size() > MAX_LIFESKILL_LEVEL)
		{
			return false;
		}

		//	read max level
		LifeSkillInfo info = {0};
		info.nSkillType = i;
		info.nMaxLevel = atoi(xSections[0].c_str());

		if(0 == info.nMaxLevel)
		{
			return false;
		}

		//	check size
		if(info.nMaxLevel != xSections.size())
		{
			return false;
		}

		//	read levels
		for(int k = 1; k < xSections.size(); ++k)
		{
			short sLevel = atoi(xSections[k].c_str());
			if(0 == sLevel)
			{
				return false;
			}

			info.uExp[k - 1] = sLevel;
		}

		//	ok
		m_ayLifeSkills[i] = info;
	}

	//	load make equip
	CSimpleIniA::TNamesDepend xMakeEquipKeys;
	if(xIniFile.GetAllKeys("MAKEEQUIPDEF", xMakeEquipKeys))
	{
		CSimpleIniA::TNamesDepend::const_iterator begIter = xMakeEquipKeys.begin();
		CSimpleIniA::TNamesDepend::const_iterator endIter = xMakeEquipKeys.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			const CSimpleIniA::Entry& refEntry = *begIter;

			const char* pszKey = refEntry.pItem;
			if(NULL == pszKey ||
				pszKey[0] == 0)
			{
				continue;
			}

			const char* pszValue = xIniFile.GetValue("MAKEEQUIPDEF", pszKey, "");
			if(NULL == pszValue ||
				pszValue[0] == 0)
			{
				continue;
			}

			int nItemId = atoi(pszKey);
			if(0 == nItemId)
			{
				continue;
			}

			//	split it
			string xWaitSplit = pszValue;
			vector<string> xSections;
			split(xWaitSplit, xSections, "|");

			int nSize = xSections.size();
			if(nSize > 5 + 1)
			{
				nSize = 5 + 1;
			}
			if(nSize <= 1)
			{
				continue;
			}

			MakeEquipInfo info = {0};
			info.nItemId = nItemId;

			//	read needLevel and exp
			vector<string> xNeedLevelAndExp;
			split(xSections[0], xNeedLevelAndExp, ",");
			if(2 != xNeedLevelAndExp.size())
			{
				continue;
			}
			info.nNeedLevel = atoi(xNeedLevelAndExp[0].c_str());
			info.nExp = atoi(xNeedLevelAndExp[1].c_str());

			//	read materials
			int nMaterialIndex = 0;
			for(int i = 1; i < nSize; ++i)
			{
				vector<string> xMaterials;
				split(xSections[i], xMaterials, ",");

				if(2 != xMaterials.size())
				{
					continue;
				}

				int nMaterialId = atoi(xMaterials[0].c_str());
				int nMaterialCount = atoi(xMaterials[1].c_str());

				if(0 == nMaterialCount ||
					0 == nMaterialId)
				{
					continue;
				}

				
				info.nMaterialsId[nMaterialIndex] = nMaterialId;
				info.nMaterialsCount[nMaterialIndex] = nMaterialCount;
				++nMaterialIndex;
			}

			if(0 != nMaterialIndex)
			{
				info.nMaterialCount = nMaterialIndex;
				m_xMakeEquipMap.insert(std::make_pair(nItemId, info));
			}
		}
	}

	xIniFile.Reset();
	return true;
}

const LifeSkillInfo* StoveManager::GetLifeSkillInfo(LifeSkillType _eType)
{
	return &m_ayLifeSkills[_eType];
}