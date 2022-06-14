#include "stdafx_Core.h"

namespace CoreEngine
{
	C_Sprite::C_Sprite()
	{
		m_pDxSprite = nullptr;
		m_pDxLine = nullptr;
	}

	C_Sprite::~C_Sprite()
	{

	}

	HRESULT C_Sprite::Create(LPD3DXSPRITE _pSprite, LPD3DXLINE _pLine)
	{
		NULL_CHECK(_pSprite, "_pSprite is nullptr");
		NULL_CHECK(_pLine, "_pLine is nullptr");

		m_pDxSprite = _pSprite;
		m_pDxLine = _pLine;

		return S_OK;
	}

	HRESULT C_Sprite::Draw(
		LPDIRECT3DTEXTURE9	_pTex,
		const RECT*			_pSrcRect,
		const D3DXVECTOR2*	_pScaling,		// Scaling
		const D3DXVECTOR2*	_pCenter,		// Rotation Center
		FLOAT				_fAngle,		// Degree.
		const D3DXVECTOR2*	_pPosition,		// Translation
		const D3DXVECTOR3*	_pOffset,		// Offset
		D3DXCOLOR			_Color
	)
	{
		NULL_CHECK(m_pDxSprite, "m_pDxSprite is nullptr");

		m_pDxSprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX mtW;
		D3DXMatrixIdentity(&mtW);

		FLOAT			fTheta = D3DXToRadian(_fAngle);
		D3DXMATRIX		mtScl;		// Scaling Matrix
		D3DXMATRIX		mtRot;		// Rotation Matrix
		D3DXMATRIX		mtTrn;		// Translation Matrix
		D3DXMATRIX		mtRct;		// Rotation Center Matrix
		D3DXMATRIX		mtRctI;		// Rotation Center Matrix Inverse

		D3DXMatrixIdentity(&mtScl);
		D3DXMatrixIdentity(&mtRct);
		D3DXMatrixIdentity(&mtRot);
		D3DXMatrixIdentity(&mtTrn);
		D3DXMatrixIdentity(&mtRctI);

		if (_pScaling)
			D3DXMatrixScaling(&mtScl, _pScaling->x, _pScaling->y, 1);

		D3DXMatrixRotationZ(&mtRot, -fTheta);

		if (_pCenter)
			D3DXMatrixTranslation(&mtRct, _pCenter->x, _pCenter->y, 0);

		//	D3DXMatrixInverse(&mtRctI, NULL, &mtRct);
		if (_pCenter)
			D3DXMatrixTranslation(&mtRctI, -(_pCenter->x), -(_pCenter->y), 0);

		if (_pPosition)
			D3DXMatrixTranslation(&mtTrn, _pPosition->x, _pPosition->y, 0);

		mtW = mtScl * mtRctI * mtRot * mtRct * mtTrn;

		m_pDxSprite->SetTransform(&mtW);

		m_pDxSprite->Draw(_pTex, _pSrcRect, _pOffset, NULL, _Color);

		D3DXMatrixIdentity(&mtW);
		m_pDxSprite->SetTransform(&mtW);

		m_pDxSprite->End();

		return S_OK;
	}
	HRESULT C_Sprite::Draw(
		LPDIRECT3DTEXTURE9	_pTex,
		const RECT*			_pSrcRect,
		const D3DXVECTOR2*	_pScaling,		// Scaling
		const D3DXVECTOR3*	_pRotation,		// Rotation
		const D3DXVECTOR2*	_pPosition,		// Translation
		const D3DXVECTOR3*	_pOffset,		// Offset
		D3DXCOLOR			_Color
	)
	{
		NULL_CHECK(m_pDxSprite, "m_pDxSprite is nullptr");

		if (!_pRotation)
			return this->Draw(_pTex, _pSrcRect, _pScaling, nullptr, 0.0f, _pPosition, _pOffset, _Color);

		D3DXVECTOR2 rotation(_pRotation->x, _pRotation->y);
		return this->Draw(_pTex, _pSrcRect, _pScaling, &rotation, _pRotation->z, _pPosition, _pOffset, _Color);
	}
	HRESULT C_Sprite::DrawLine(
		const D3DXVECTOR2* _pVertexList,
		DWORD				_dwVertexListCount,
		D3DCOLOR			_Color,
		FLOAT				_fWidth,
		bool				_bAntialias
	)
	{
		NULL_CHECK(m_pDxLine, "m_pDxLine is nullptr");

		m_pDxLine->SetAntialias(_bAntialias);
		m_pDxLine->SetWidth(_fWidth);

		m_pDxLine->Begin();

		m_pDxLine->Draw(_pVertexList, _dwVertexListCount, _Color);

		m_pDxLine->End();

		return S_OK;
	}
}