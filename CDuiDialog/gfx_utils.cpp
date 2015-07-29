#include "gfx_utils.h"
#include <hgesprite.h>
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
#define RECT_WIDTH(RC)	(RC.right - RC.left)
#define RECT_HEIGHT(RC)	(RC.bottom - RC.top)
//////////////////////////////////////////////////////////////////////////
void Gfx_Render9Path(hgeSprite* _pSpr, HTEXTURE _tex, const RECT* _pTexRect, const RECT* _p9Rect, float _fx, float _fy, float _fw, float _fh)
{
	if(NULL == _pTexRect ||
		NULL == _p9Rect)
	{
		assert("null text rect or 9path rect" == NULL);
		return;
	}
	if(NULL == _pSpr)
	{
		assert("null sprite" == NULL);
		return;
	}

	//	计算出九宫格对应的矩形区域
	RECT rc9Path[9] = {0};

	//	left top
	rc9Path[0].left = _pTexRect->left;
	rc9Path[0].top = _pTexRect->top;
	rc9Path[0].right = _pTexRect->left + _p9Rect->left;
	rc9Path[0].bottom = _pTexRect->top + _p9Rect->top;

	//	center top
	rc9Path[1].left = _pTexRect->left + _p9Rect->left;
	rc9Path[1].top = _pTexRect->top;
	rc9Path[1].right = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[1].bottom = _pTexRect->top + _p9Rect->top;

	//	right top
	rc9Path[2].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[2].top = _pTexRect->top;
	rc9Path[2].right = _pTexRect->left + _pTexRect->right;
	rc9Path[2].bottom = _pTexRect->top + _p9Rect->top;

	//	center left
	rc9Path[3].left = _pTexRect->left;
	rc9Path[3].top = _pTexRect->top + _p9Rect->top;
	rc9Path[3].right = _pTexRect->left + _p9Rect->left;
	rc9Path[3].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	center center
	rc9Path[4].left = _pTexRect->left + _p9Rect->left;
	rc9Path[4].top = _pTexRect->top + _p9Rect->top;
	rc9Path[4].right = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[4].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	center right
	rc9Path[5].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[5].top = _pTexRect->top + _p9Rect->top;
	rc9Path[5].right = _pTexRect->left + _pTexRect->right;
	rc9Path[5].bottom = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;

	//	bottom left
	rc9Path[6].left = _pTexRect->left;
	rc9Path[6].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[6].right = _pTexRect->left + _p9Rect->left;
	rc9Path[6].bottom = _pTexRect->top + _pTexRect->bottom;
	
	//	bottom center
	rc9Path[7].left = _pTexRect->left + _p9Rect->left;
	rc9Path[7].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[7].right = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[7].bottom = _pTexRect->top + _pTexRect->bottom;

	//	bottom right
	rc9Path[8].left = _pTexRect->left + _pTexRect->right - _p9Rect->right;
	rc9Path[8].top = _pTexRect->top + _pTexRect->bottom - _p9Rect->bottom;
	rc9Path[8].right = _pTexRect->left + _pTexRect->right;
	rc9Path[8].bottom = _pTexRect->top + _pTexRect->bottom;

	//	设置不同的纹理区域来渲染
	_pSpr->SetTexture(_tex);

	//	left top
	_pSpr->SetTextureRect(rc9Path[0].left,
		rc9Path[0].top,
		RECT_WIDTH(rc9Path[0]),
		RECT_HEIGHT(rc9Path[0]));
	_pSpr->Render(_fx, _fy);

	//	center top
	_pSpr->SetTextureRect(rc9Path[1].left,
		rc9Path[1].top,
		RECT_WIDTH(rc9Path[1]),
		RECT_HEIGHT(rc9Path[1]));
	_pSpr->RenderStretch(_fx + _p9Rect->left,
		_fy,
		_fx + _fw - _p9Rect->right,
		_fy + _p9Rect->top);

	//	right top
	_pSpr->SetTextureRect(rc9Path[2].left,
		rc9Path[2].top,
		RECT_WIDTH(rc9Path[2]),
		RECT_HEIGHT(rc9Path[2]));
	_pSpr->Render(_fx + _fw - _p9Rect->right,
		_fy);

	//	left center
	_pSpr->SetTextureRect(rc9Path[3].left,
		rc9Path[3].top,
		RECT_WIDTH(rc9Path[3]),
		RECT_HEIGHT(rc9Path[3]));
	_pSpr->RenderStretch(_fx,
		_fy + _p9Rect->top,
		_fx + _p9Rect->left,
		_fy + _fh - _p9Rect->bottom);

	//	center center
	_pSpr->SetTextureRect(rc9Path[4].left,
		rc9Path[4].top,
		RECT_WIDTH(rc9Path[4]),
		RECT_HEIGHT(rc9Path[4]));
	_pSpr->RenderStretch(_fx + _p9Rect->left,
		_fy + _p9Rect->top,
		_fx + _fw - _p9Rect->right,
		_fy + _fh - _p9Rect->bottom);

	//	right center
	_pSpr->SetTextureRect(rc9Path[5].left,
		rc9Path[5].top,
		RECT_WIDTH(rc9Path[5]),
		RECT_HEIGHT(rc9Path[5]));
	_pSpr->RenderStretch(_fx + _fw - _p9Rect->right,
		_fy + _p9Rect->top,
		_fx + _fw,
		_fy + _fh - _p9Rect->bottom);

	//	left bottom
	_pSpr->SetTextureRect(rc9Path[6].left,
		rc9Path[6].top,
		RECT_WIDTH(rc9Path[6]),
		RECT_HEIGHT(rc9Path[6]));
	_pSpr->Render(_fx,
		_fy + _fh - _p9Rect->bottom);

	//	center bottom
	_pSpr->SetTextureRect(rc9Path[7].left,
		rc9Path[7].top,
		RECT_WIDTH(rc9Path[7]),
		RECT_HEIGHT(rc9Path[7]));
	_pSpr->RenderStretch(_fx + _p9Rect->left,
		_fy + _fh - _p9Rect->bottom,
		_fx + _fw - _p9Rect->right,
		_fy + _fh);

	//	right bottom
	_pSpr->SetTextureRect(rc9Path[8].left,
		rc9Path[8].top,
		RECT_WIDTH(rc9Path[8]),
		RECT_HEIGHT(rc9Path[8]));
	_pSpr->Render(_fx + _fw - _p9Rect->right,
		_fy + _fh - _p9Rect->bottom);
}