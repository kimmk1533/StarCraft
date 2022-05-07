#pragma once

namespace CoreEngine
{
	class C_Sprite
	{
	protected:
		LPD3DXSPRITE	m_pDxSprite;		// 2D DX Sprite

	public:
		C_Sprite();
		virtual ~C_Sprite();

		HRESULT		Create(LPD3DXSPRITE);

		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	pTex,
			const RECT* pSrcRect,
			const D3DXVECTOR2* pScaling,	// Scaling
			const D3DXVECTOR2* pCenter,	// Rotation Center
			FLOAT				fAngle,		// Degree.
			const D3DXVECTOR2* pPosition,	// Translation
			const D3DXVECTOR3* pOffset,	// Offset
			D3DXCOLOR			Color
		);
		HRESULT		Draw(
			LPDIRECT3DTEXTURE9	pTex,
			const RECT* pSrcRect,
			const D3DXVECTOR2* pScaling,	// Scaling
			const D3DXVECTOR3* pRotation,	// Rotation
			const D3DXVECTOR2* pPosition,	// Translation
			const D3DXVECTOR3* pOffset,	// Offset
			D3DXCOLOR			Color
		);
	};
}