#pragma once

namespace CoreEngine
{
	class C_Sprite
	{
	protected:
		LPD3DXSPRITE	m_pDxSprite;		// 2D DX Sprite
		LPD3DXLINE		m_pDxLine;

	public:
		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScale,	// Scaling
			const D3DXVECTOR2*	_pCenter,	// Rotation Center
			const FLOAT&		_fAngle,	// Degree.
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			const D3DXCOLOR&	_color		// Color
		);
		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScale,	// Scaling
			const D3DXVECTOR3*	_pRotation,	// Rotation
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			const D3DXCOLOR&	_color		// Color
		);

		HRESULT		DrawLine(
			const D3DXVECTOR2*	_pVertexList,
			const DWORD&		_dwVertexListCount,
			const FLOAT&		_fThickness,
			const bool&			_bAntialias,
			const D3DCOLOR&		_color
		);
		HRESULT		DrawRect(
			const RECT&			_rect,
			const FLOAT&		_fThickness,
			const bool&			_bAntialias,
			const D3DCOLOR&		_color
		);
		HRESULT		DrawEllipse(
			const FLOAT&		_fCenterX,
			const FLOAT&		_fCenterY,
			const FLOAT&		_fWidth,
			const FLOAT&		_fHeight,
			const FLOAT&		_fNumSides,
			const FLOAT&		_fThickness,
			const D3DCOLOR&		_color
		);

	public:
		C_Sprite();
		virtual ~C_Sprite();

	public:
		HRESULT		Create(LPD3DXSPRITE _pSprite, LPD3DXLINE _pLine);

	};
}