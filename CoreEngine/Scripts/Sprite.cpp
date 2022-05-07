#include "stdafx.h"

namespace CoreEngine
{
	C_Sprite::C_Sprite()
	{
		m_pDxSprite = nullptr;
	}

	C_Sprite::~C_Sprite()
	{

	}

	HRESULT C_Sprite::Create(LPD3DXSPRITE pSprite)
	{
		m_pDxSprite = pSprite;

		return S_OK;
	}

	HRESULT C_Sprite::Draw(
		LPDIRECT3DTEXTURE9	pTex,
		const RECT* pSrcRect,
		const D3DXVECTOR2* pScaling,		// Scaling
		const D3DXVECTOR2* pCenter,		// Rotation Center
		FLOAT				fAngle,			// Degree.
		const D3DXVECTOR2* pPosition,		// Translation
		const D3DXVECTOR3* pOffset,		// Offset
		D3DXCOLOR			Color
	)
	{
		m_pDxSprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX mtW;
		D3DXMatrixIdentity(&mtW);

		FLOAT			fTheta = D3DXToRadian(fAngle);
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

		if (pScaling)
			D3DXMatrixScaling(&mtScl, pScaling->x, pScaling->y, 1);

		D3DXMatrixRotationZ(&mtRot, -fTheta);

		if (pCenter)
			D3DXMatrixTranslation(&mtRct, pCenter->x, pCenter->y, 0);

		//	D3DXMatrixInverse(&mtRctI, NULL, &mtRct);
		if (pCenter)
			D3DXMatrixTranslation(&mtRctI, -pCenter->x, -pCenter->y, 0);

		if (pPosition)
			D3DXMatrixTranslation(&mtTrn, pPosition->x, pPosition->y, 0);

		mtW = mtScl * mtRctI * mtRot * mtRct * mtTrn;

		m_pDxSprite->SetTransform(&mtW);

		m_pDxSprite->Draw(pTex, pSrcRect, pOffset, NULL, Color);

		D3DXMatrixIdentity(&mtW);
		m_pDxSprite->SetTransform(&mtW);

		m_pDxSprite->End();

		return S_OK;
	}
	HRESULT C_Sprite::Draw(
		LPDIRECT3DTEXTURE9	pTex,
		const RECT* pSrcRect,
		const D3DXVECTOR2* pScaling,		// Scaling
		const D3DXVECTOR3* pRotation,		// Rotation
		const D3DXVECTOR2* pPosition,		// Translation
		const D3DXVECTOR3* pOffset,		// Offset
		D3DXCOLOR			Color
	)
	{
		if (!pRotation)
			return this->Draw(pTex, pSrcRect, pScaling, nullptr, 0.0f, pPosition, pOffset, Color);

		D3DXVECTOR2 rotation(pRotation->x, pRotation->y);
		return this->Draw(pTex, pSrcRect, pScaling, &rotation, pRotation->z, pPosition, pOffset, Color);
	}
}