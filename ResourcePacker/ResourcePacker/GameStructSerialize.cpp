#include "GameStructSerialize.h"
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* ItemAttrib
/************************************************************************/
/*
BYTEBUFFER_STRUCT_READ(ItemAttrib)
{
	_xBuffer
		>> _var.AC
		>> _var.accuracy
		>> _var.atkPalsy
		>> _var.atkPois
		>> _var.atkSpeed
		>> _var.curse
		>> _var.DC
		>> _var.EXPR
		>> _var.extra
		>> _var.hide
		>> _var.HP
		>> _var.id
		>> _var.level
		>> _var.lucky
		>> _var.MAC
		>> _var.maxAC
		>> _var.maxDC
		>> _var.maxEXPR
		>> _var.maxHP
		>> _var.maxMAC
		>> _var.maxMC
		>> _var.maxMP
		>> _var.maxSC
		>> _var.MC
		>> _var.moveSpeed
		>> _var.MP
		>> _var.name
		>> _var.reqType
		>> _var.reqValue
		>> _var.SC
		>> _var.sex
		>> _var.tex
		>> _var.type
		>> _var.weight
		>> _var.price
		//	Extra, not in database
		>> _var.tag;

	return _xBuffer;
}
BYTEBUFFER_STRUCT_WRITE(ItemAttrib)
{
	_xBuffer
		<< _var.AC
		<< _var.accuracy
		<< _var.atkPalsy
		<< _var.atkPois
		<< _var.atkSpeed
		<< _var.curse
		<< _var.DC
		<< _var.EXPR
		<< _var.extra
		<< _var.hide
		<< _var.HP
		<< _var.id
		<< _var.level
		<< _var.lucky
		<< _var.MAC
		<< _var.maxAC
		<< _var.maxDC
		<< _var.maxEXPR
		<< _var.maxHP
		<< _var.maxMAC
		<< _var.maxMC
		<< _var.maxMP
		<< _var.maxSC
		<< _var.MC
		<< _var.moveSpeed
		<< _var.MP
		<< _var.name
		<< _var.reqType
		<< _var.reqValue
		<< _var.SC
		<< _var.sex
		<< _var.tex
		<< _var.type
		<< _var.weight
		<< _var.price
		//	Extra,not in database
		<< _var.tag;

	return _xBuffer;
}

BYTEBUFFER_STRUCT_READ(UserData)
{
	_xBuffer
		>> _var.stAttrib
		>> _var.wCoordX
		>> _var.wCoordY
		>> _var.wMapID
		>> _var.nDrt;
	return _xBuffer;
}

BYTEBUFFER_STRUCT_WRITE(UserData)
{
	_xBuffer
		<< _var.stAttrib
		<< _var.wCoordX
		<< _var.wCoordY
		<< _var.wMapID
		<< _var.nDrt;
	return _xBuffer;
}

BYTEBUFFER_STRUCT_READ(DelaySendInfo)
{
	_xBuffer
		>> _var.uOp
		>> _var.uDelayTime;
	for(int i = 0; i < sizeof(_var.uParam) / sizeof(_var.uParam[0]); ++i)
	{
		_xBuffer >> _var.uParam[i];
	}
	return _xBuffer;
}

BYTEBUFFER_STRUCT_WRITE(DelaySendInfo)
{
	_xBuffer
		<< _var.uOp
		<< _var.uDelayTime;
	for(int i = 0; i < sizeof(_var.uParam) / sizeof(_var.uParam[0]); ++i)
	{
		_xBuffer << _var.uParam[i];
	}
	return _xBuffer;
}*/


BYTEBUFFER_STRUCT_READ(HeroHeader)
{
	_xBuffer >> _var.szName
		>> _var.bSex
		>> _var.bJob
		>> _var.uLevel;
	return _xBuffer;
}
BYTEBUFFER_STRUCT_WRITE(HeroHeader)
{
	_xBuffer << _var.szName
		<< _var.bSex
		<< _var.bJob
		<< _var.uLevel;
	return _xBuffer;
}