#ifndef INITOSQL_H_
#define INITOSQL_H_
#include <sqlite\sqlite3.h>
#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include "SKnl3Helper.h"

//	装备属性
struct ItemAttrib
{
	//	装备代号 唯一ID
	UINT id;	//	人物
	//	装备名字
	char name[20];
	//	幸运值
	BYTE lucky;	//	对于药品 0:分成5份加 1:直接加 对于卷轴 0:随机 2:回程
	//	诅咒值
	BYTE curse;	//	对于怪物 是否在地下 0 == 在地下 1 == 在地面   辨别ITEM_COST类别 0:药品 1:卷轴
	//	躲避
	BYTE hide;	//	
	//	准确
	BYTE accuracy;
	//	攻击速度
	BYTE atkSpeed;
	//	麻痹几率
	BYTE atkPalsy;
	//	中毒几率
	BYTE atkPois;
	//	移动速度
	BYTE moveSpeed;
	//	重量
	BYTE weight;
	//	要求类型 1 = 等级 2 = 攻击 3 = 魔法力 4 = 道术 5 = 性别
	BYTE reqType;
	//	要求值	0 = 通用 0 = 战士 1 = 法师 2 = 道士   ||0 = 男 1 = 女
	BYTE reqValue;
	//	男女
	BYTE sex;
	//	种类
	BYTE type;
	//	攻击力
	USHORT maxDC;
	USHORT DC;
	//	防御力
	USHORT maxAC;
	USHORT AC;
	//	魔防
	USHORT maxMAC;
	USHORT MAC;
	//	道术
	USHORT maxSC;//	用于确定怪物的攻击距离
	USHORT SC;//	用于确定怪物视野
	//	魔法力
	USHORT maxMC;	//	用于确定怪物是否处于仇恨状态
	USHORT MC;		//	用于确定怪物是否主动攻击怪物
	//	血量
	UINT maxHP;
	UINT HP;
	//	魔
	UINT maxMP;	
	UINT MP;	
	//	经验
	UINT maxEXPR;
	UINT EXPR;
	//	装备的要求等级 在reqType上面
	USHORT level;
	//	附加
	USHORT extra;
	//	图片索引  用于怪物和人物的时候 表示地图ID
	USHORT tex;

	//	价格	//	人物的话 是职业
	USHORT price;
};

int IniFileTransToSqlFile(const char* lpszCfg, sqlite3* pSql);
int SqlFileTransToIniFile(const char* lpszCfg, sqlite3* pSql);


int IniFileTransToSqlFileMain();
int SqlFileTransToIniFileMain();

bool Ini2SqlDropItem(sqlite3* _pSql, const char* _pszFilename);
bool Ini2SqlMonsters(sqlite3* _pSql, const char* _pszFilename);
bool Ini2SqlItems(sqlite3* _pSql, const char* _pszFilename);
bool Ini2SqlMagic(sqlite3* _pSql, const char* _pszFilename);

void GetIniFileSections(const char* _pszFile, std::vector<int>& _xOutput);
void GetIniFileKeys(const char* _pszAppName, const char* _pszFile, std::vector<int>& _xOutput);

bool GetNameToIDMap(const char* _pszFile, std::map<std::string, int>& _xOutput);
bool ParseDropItem(const char* _pszFile, char* _pBuf, int _id);
bool ParseDropItem(const char* _pszFile, char* _pBuf, int _id, std::map<std::string, int>& _xNameIDMap);

struct DBCallbackParam
{
	ItemAttrib* pItem;
	const char* szOutputFile;
};
int DBCallback(void* _param, int _count, char** _value, char** _name);

/*
*	获得文件大小
*/
//DWORD SKNL3GetFileSize(LPCTSTR lpszFileName);
/*
*	获得运行目录
*/
//BOOL SKNL3GetCurrentAppPath(LPSTR lpBuffer, size_t _buffer_size);

#endif