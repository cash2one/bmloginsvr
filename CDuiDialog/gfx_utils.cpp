#include "gfx_utils.h"
#include <hgesprite.h>
#include <assert.h>
//////////////////////////////////////////////////////////////////////////
#define RECT_WIDTH(RC)	(RC.right - RC.left)
#define RECT_HEIGHT(RC)	(RC.bottom - RC.top)
//////////////////////////////////////////////////////////////////////////
//	标题栏拉伸
static bool s_bGfx_9Path_CenterStretch = true;
//////////////////////////////////////////////////////////////////////////
void Gfx_SetRenderState(Gfx_RenderState _eState, int _nParam)
{
	switch(_eState)
	{
	case kGfx_9Path_CenterStretch:
		{
			s_bGfx_9Path_CenterStretch = (_nParam != 0);
		}break;
	}
}

int Gfx_GetRenderState(Gfx_RenderState _eState)
{
	switch(_eState)
	{
	case kGfx_9Path_CenterStretch:
		{
			return s_bGfx_9Path_CenterStretch ? 1 : 0;
		}break;
	}

	return 0;
}

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

	if(s_bGfx_9Path_CenterStretch)
	{
		_pSpr->RenderStretch(_fx + _p9Rect->left,
			_fy,
			_fx + _fw - _p9Rect->right,
			_fy + _p9Rect->top);
	}
	else
	{
		int nRepeatFullTimes = (_fw - _p9Rect->right - _p9Rect->left) / RECT_WIDTH(rc9Path[1]);
		int nRepeatRemainderPixels = (_fw - _p9Rect->right - _p9Rect->left) - nRepeatFullTimes * RECT_WIDTH(rc9Path[1]);

		//	render repeat parts
		float fRenderX, fRenderY = 0;
		for(int i = 0; i < nRepeatFullTimes; ++i)
		{
			fRenderX = _fx + _p9Rect->left + i * RECT_WIDTH(rc9Path[1]);
			fRenderY = _fy;
			_pSpr->Render(fRenderX, fRenderY);
		}

		//	render left part
		if(0 != nRepeatRemainderPixels)
		{
			fRenderX = _fy + _p9Rect->left + nRepeatFullTimes * RECT_WIDTH(rc9Path[1]);
			_pSpr->SetTextureRect(rc9Path[1].left,
				rc9Path[1].top,
				nRepeatRemainderPixels,
				RECT_HEIGHT(rc9Path[1]));
			_pSpr->Render(fRenderX, fRenderY);
		}
	}

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

HTEXTURE Gfx_GetBrightTexture(HGE* _pHge, HTEXTURE _hSource, int _nBrightAdd)
{
	if(NULL == _hSource)
	{
		return NULL;
	}

	int nTextureWidth = _pHge->Texture_GetWidth(_hSource);
	int nTextureHeight = _pHge->Texture_GetHeight(_hSource);

	if(0 == nTextureHeight ||
		0 == nTextureWidth)
	{
		return NULL;
	}

	HTEXTURE texNew = _pHge->Texture_Create(nTextureWidth, nTextureHeight);
	DWORD* pDestData = _pHge->Texture_Lock(texNew, false);
	DWORD* pSrcData = _pHge->Texture_Lock(_hSource);

	for(int nRow = 0; nRow < nTextureHeight; ++nRow)
	{
		for(int nCol = 0; nCol < nTextureWidth; ++nCol)
		{
			DWORD dwPixelIndex = nRow * nTextureWidth + nCol;
			DWORD dwPixel = pSrcData[dwPixelIndex];
			BYTE bAlpha = (dwPixel & 0xff000000) >> 24;

			if(bAlpha)
			{
				//	取出RGB值
				BYTE bR = (dwPixel & 0x00ff0000) >> 16;
				BYTE bG = (dwPixel & 0x0000ff00) >> 8;
				BYTE bB = dwPixel & 0x000000ff;

				if(0 == bR &&
					0 == bG &&
					0 == bB)
				{
					continue;
				}
				
				//	转为YUV
				float fY = 0.299f * (float)bR + 0.587f * (float)bG + 0.114f * (float)bB;
				float fU = 0.565f * ((float)bB - fY);
				float fV = 0.713f * ((float)bR - fY);

				//	提升Y值 提高亮度
				if(fY + _nBrightAdd > 0xff)
				{
					fY = 0xff;
				}
				else
				{
					fY = fY + _nBrightAdd;
				}

				//	转换为RGB
				int nR = fY + 1.403f * fV;
				if(nR > 0xff)
				{
					bR = 0xff;
				}
				else
				{
					bR = nR;
				}
				int nG = fY - 0.344f * fU - 0.714f * fV;
				if(nG > 0xff)
				{
					bG = 0xff;
				}
				else
				{
					bG = nG;
				}
				int nB = fY + 1.77f * fU;
				if(nB > 0xff)
				{
					bB = 0xff;
				}
				else
				{
					bB = nB;
				}
				pDestData[dwPixelIndex] = ARGB(bAlpha, bR, bG, bB);
			}
		}
	}

	_pHge->Texture_Unlock(_hSource);
	_pHge->Texture_Unlock(texNew);

	return texNew;
}