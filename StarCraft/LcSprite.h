#pragma once

class CLcSprite
{
protected:
	LPD3DXSPRITE	m_pDxSprite;			// 2D DX Sprite

public:
	CLcSprite();
	virtual ~CLcSprite();

	HRESULT		Create(LPD3DXSPRITE);

	HRESULT		Draw(
		LPDIRECT3DTEXTURE9 pTex,
		const RECT* pSrcRect,
		const D3DXVECTOR2* pScaling,	// Scaling
		const D3DXVECTOR2* pCenter,		// Rotation Center
		FLOAT	fAngle,					// Degree.
		const D3DXVECTOR2* pPosition,	// Translation
		D3DXCOLOR Color
	);
	HRESULT		Draw(
		LPDIRECT3DTEXTURE9 pTex,
		const RECT* pSrcRect,
		const D3DXVECTOR2* pScaling,	// Scaling
		const D3DXVECTOR3* pRotation,	// Rotation
		const D3DXVECTOR2* pPosition,	// Translation
		D3DXCOLOR Color
	);
};

