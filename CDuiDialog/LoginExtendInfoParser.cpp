#include "LoginExtendInfoParser.h"
#include "json.h"
//////////////////////////////////////////////////////////////////////////
LoginExtendInfoParser::LoginExtendInfoParser(const char* _pszContent)
{
	m_nDonateMoney = 0;
	SetContent(_pszContent);
}

void LoginExtendInfoParser::SetContent(const char* _pszContent)
{
	m_xContent = _pszContent;
}

bool LoginExtendInfoParser::Parse()
{
	m_xGiftsID.clear();
	m_nDonateMoney = 0;

	if(m_xContent.empty())
	{
		return false;
	}

	//	parse
	json_t* pRoot = NULL;
	json_error ejsonError = json_parse_document(&pRoot, m_xContent.c_str());

	if(JSON_OK != ejsonError)
	{
		return false;
	}

	bool bRet = false;
	do 
	{
		//	find donatemoney label
		json_t* pDonateMoneyLabel = json_find_first_label(pRoot, "DonateMoney");
		if(NULL == pDonateMoneyLabel ||
			JSON_STRING != pDonateMoneyLabel->type)
		{
			break;
		}

		json_t* pDonateMoneyValueLabel = pDonateMoneyLabel->child;
		if(NULL == pDonateMoneyValueLabel ||
			JSON_NUMBER != pDonateMoneyValueLabel->type)
		{
			break;
		}
		m_nDonateMoney = atoi(pDonateMoneyValueLabel->text);

		json_t* pGiftsLabel = json_find_first_label(pRoot, "SystemGift");
		if(NULL == pGiftsLabel)
		{
			break;
		}
		json_t* pGiftsValueLabel = pGiftsLabel->child;
		if(NULL == pGiftsValueLabel)
		{
			break;
		}

		//	maybe null, or an array
		if(pGiftsValueLabel->type == JSON_NULL)
		{
			//	nothing
		}
		else if(pGiftsValueLabel->type == JSON_ARRAY)
		{
			json_t* pFirstArrayElement = pGiftsValueLabel->child;
			if(NULL == pFirstArrayElement ||
				JSON_NUMBER != pFirstArrayElement->type)
			{
				break;
			}

			json_t* pArrayItem = pFirstArrayElement;
			int nArraySize = 0;

			while(NULL != pArrayItem)
			{
				pArrayItem = pArrayItem->next;
				++nArraySize;
			}

			if(0 != nArraySize)
			{
				m_xGiftsID.resize(nArraySize, 0);
			}

			pArrayItem = pFirstArrayElement;
			nArraySize = 0;

			while(NULL != pArrayItem)
			{
				m_xGiftsID[nArraySize] = atoi(pArrayItem->text);
				pArrayItem = pArrayItem->next;
				++nArraySize;
			}
		}

		bRet = true;
	} while (0);

	json_free_value(&pRoot);
	return bRet;
}

int LoginExtendInfoParser::GetDonateMoney()
{
	return m_nDonateMoney;
}

int LoginExtendInfoParser::GetGiftCount()
{
	return m_xGiftsID.size();
}

int LoginExtendInfoParser::GetGiftID(int _nIndex)
{
	if(_nIndex < 0 ||
		_nIndex >= GetGiftCount())
	{
		return 0;
	}

	return m_xGiftsID[_nIndex];
}
//////////////////////////////////////////////////////////////////////////