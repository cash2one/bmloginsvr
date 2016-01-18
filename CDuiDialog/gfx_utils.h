#ifndef _INC_GFXUTILS_
#define _INC_GFXUTILS_
//////////////////////////////////////////////////////////////////////////
//	include
#include <Windows.h>
#include <hge.h>
//////////////////////////////////////////////////////////////////////////
//	forward declaration
class hgeSprite;
//////////////////////////////////////////////////////////////////////////
//	statement
enum Gfx_RenderState
{
	kGfx_9Path_CenterStretch,
};

void Gfx_SetRenderState(Gfx_RenderState _eState, int _nParam);
int Gfx_GetRenderState(Gfx_RenderState _eState);
void Gfx_Render9Path(hgeSprite* _pSpr, HTEXTURE _tex, const RECT* _pTexRect, const RECT* _p9Rect, float _fx, float _fy, float _fw, float _fh);
HTEXTURE Gfx_GetBrightTexture(HGE* _pHge, HTEXTURE _hSource, int _nBrightAdd);
HTEXTURE Gfx_GetOutlineTexture(HGE* _pHge, HTEXTURE _tex, int _nSize, DWORD _dwOutlineColor);
//////////////////////////////////////////////////////////////////////////
#endif