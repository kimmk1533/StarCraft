#pragma once

namespace CoreEngine
{
	class C_Sprite : public C_FrameWork
	{
	protected:
		LPD3DXSPRITE	m_pDxSprite;		// 2D DX Sprite
		LPD3DXLINE		m_pDxLine;

		D3DXMATRIX*		m_pMtTranslation;
		D3DXMATRIX*		m_pMtRotation;
		D3DXMATRIX*		m_pMtScale;
		D3DXMATRIX*		m_pMtSRT;
		D3DXMATRIX*		m_pMtResult;

	public:
		HRESULT		SetTranslation(const D3DXVECTOR2& _pos);
		HRESULT		SetTranslation(const D3DXVECTOR3& _pos);
		HRESULT		SetTranslation(const D3DXVECTOR3* _pos);
		HRESULT		SetTranslation(const FLOAT& _x, const FLOAT& _y, const FLOAT& _z);

		HRESULT		SetRotation(const D3DXVECTOR2& _rotCenter, const FLOAT& _angle);
		HRESULT		SetRotation(const D3DXVECTOR2* _rotCenter, const FLOAT& _angle);
		HRESULT		SetRotation(const D3DXVECTOR3& _rot);
		HRESULT		SetRotation(const D3DXVECTOR3* _rot);
		HRESULT		SetRotation(const FLOAT& _cx, const FLOAT& _cy, const FLOAT& _angle);

		HRESULT		SetScale(const D3DXVECTOR2& _scale);
		HRESULT		SetScale(const D3DXVECTOR3& _scale);
		HRESULT		SetScale(const D3DXVECTOR3* _scale);
		HRESULT		SetScale(const FLOAT& _x, const FLOAT& _y, const FLOAT& _z);

		HRESULT		Draw(LPD3DXTEXTURE9 _pTex, const RECT* _pSrcRect, const D3DXVECTOR3* _pCenter, const D3DCOLOR _color);
		/*HRESULT		Draw(
			LPD3DXTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScale,	// Scaling
			const D3DXVECTOR2*	_pCenter,	// Rotation Center
			const FLOAT&		_fAngle,	// Degree.
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			const D3DXCOLOR&	_color		// Color
		);
		HRESULT		Draw(
			LPD3DXTEXTURE9	_pTex,
			const RECT*			_pSrcRect,
			const D3DXVECTOR2*	_pScale,	// Scaling
			const D3DXVECTOR3*	_pRotation,	// Rotation
			const D3DXVECTOR2*	_pPosition,	// Translation
			const D3DXVECTOR3*	_pOffset,	// Offset
			const D3DXCOLOR&	_color		// Color
		);*/

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
		virtual HRESULT Create() override;
		virtual HRESULT	Create(const LPD3DXSPRITE _pSprite, const LPD3DXLINE _pLine);
		virtual void	Destroy() override;

	};
}