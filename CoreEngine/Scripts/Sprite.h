#pragma once

namespace CoreEngine
{
	class C_Sprite
	{
	public:
		C_Sprite();
		virtual ~C_Sprite();

	public:
		HRESULT		Create(LPD3DXSPRITE _pSprite, LPD3DXLINE _pLine);

	protected:
		LPD3DXSPRITE	m_pDxSprite;		// 2D DX Sprite
		LPD3DXLINE		m_pDxLine;

	public:
		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScaling,	// Scaling
			const D3DXVECTOR2*	_pCenter,	// Rotation Center
			FLOAT				_fAngle,		// Degree.
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			D3DXCOLOR			_Color
		);
		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScaling,	// Scaling
			const D3DXVECTOR3*	_pRotation,	// Rotation
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			D3DXCOLOR			_Color
		);

		HRESULT		DrawLine(
			const D3DXVECTOR2*	_pVertexList,
			DWORD				_dwVertexListCount,
			D3DCOLOR			_Color,
			FLOAT				_fWidth,
			bool				_bAntialias
		);
	};
}