#include "stdafx_Core.h"

namespace CoreEngine
{
	HRESULT C_Sprite::SetTranslation(const D3DXVECTOR2& _pos)
	{
		return SetTranslation(_pos.x, _pos.y, 0.0f);
	}
	HRESULT C_Sprite::SetTranslation(const D3DXVECTOR3& _pos)
	{
		return SetTranslation(_pos.x, _pos.y, _pos.z);
	}
	HRESULT C_Sprite::SetTranslation(const D3DXVECTOR3* _pos)
	{
		if (nullptr == _pos)
			return SetTranslation(0.0f, 0.0f, 0.0f);

		return SetTranslation(_pos->x, _pos->y, _pos->z);
	}
	HRESULT C_Sprite::SetTranslation(const FLOAT& _x, const FLOAT& _y, const FLOAT& _z)
	{
		D3DXMatrixTranslation(m_pMtPosition, _x, _y, _z);

		return E_NOTIMPL;
	}

	HRESULT	C_Sprite::SetRotation(const D3DXVECTOR2& _rotCenter, const FLOAT& _angle)
	{
		if (_angle == 0.0f)
			return SetRotation(0.0f, 0.0f, 0.0f);

		return SetRotation(_rotCenter.x, _rotCenter.y, _angle);
	}
	HRESULT C_Sprite::SetRotation(const D3DXVECTOR2* _rotCenter, const FLOAT& _angle)
	{
		if (nullptr == _rotCenter || _angle == 0.0f)
			return SetRotation(0.0f, 0.0f, 0.0f);

		return SetRotation(_rotCenter->x, _rotCenter->y, _angle);
	}
	HRESULT C_Sprite::SetRotation(const D3DXVECTOR3& _rot)
	{
		if (_rot.z == 0.0f)
			return SetRotation(0.0f, 0.0f, 0.0f);

		return SetRotation(_rot.x, _rot.y, _rot.z);
	}
	HRESULT C_Sprite::SetRotation(const D3DXVECTOR3* _rot)
	{
		if (nullptr == _rot || _rot->z == 0.0f)
			return SetRotation(0.0f, 0.0f, 0.0f);

		return SetRotation(_rot->x, _rot->y, _rot->z);
	}
	HRESULT C_Sprite::SetRotation(const FLOAT& _cx, const FLOAT& _cy, const FLOAT& _angle)
	{
		D3DXMatrixTranslation(m_pMtRotation, _cx, _cy, 0.0f);
		D3DXMatrixRotationZ(m_pMtRotation, _angle);

		return S_OK;
	}

	HRESULT C_Sprite::SetScale(const D3DXVECTOR2& _scale)
	{
		return SetScale(_scale.x, _scale.y, 1.0f);
	}
	HRESULT C_Sprite::SetScale(const D3DXVECTOR3& _scale)
	{
		return SetScale(_scale.x, _scale.y, _scale.z);
	}
	HRESULT C_Sprite::SetScale(const D3DXVECTOR3* _scale)
	{
		if (nullptr == _scale)
			return SetScale(1.0f, 1.0f, 1.0f);

		return SetScale(_scale->x, _scale->y, _scale->z);
	}
	HRESULT C_Sprite::SetScale(const FLOAT& _x, const FLOAT& _y, const FLOAT& _z)
	{
		D3DXMatrixScaling(m_pMtScale, _x, _y, _z);

		return S_OK;
	}

	HRESULT C_Sprite::Draw(LPD3DXTEXTURE9 _pTex, const RECT* _pSrcRect, const D3DXVECTOR3* _pCenter, const D3DCOLOR _color)
	{
		NULL_CHECK_WITH_MSG(m_pDxSprite, "m_pDxSprite is nullptr");

		m_pDxSprite->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMatrixIdentity(m_pMtSRT);
		D3DXMatrixIdentity(m_pMtResult);

		*m_pMtSRT = (*m_pMtScale) * (*m_pMtRotation) * (*m_pMtPosition);

		*m_pMtResult = *m_pMtSRT * (*Camera->viewMatrix);

		m_pDxSprite->SetTransform(m_pMtResult);

		m_pDxSprite->Draw(_pTex, _pSrcRect, _pCenter, nullptr, _color);

		D3DXMatrixIdentity(m_pMtResult);
		m_pDxSprite->SetTransform(m_pMtResult);

		m_pDxSprite->End();

		return S_OK;
	}

	/*HRESULT C_Sprite::Draw(
		LPD3DXTEXTURE9	_pTex,
		const RECT* _pSrcRect,
		const D3DXVECTOR2* _pScale,	// Scaling
		const D3DXVECTOR2* _pCenter,	// Rotation Center
		const FLOAT& _fAngle,	// Degree.
		const D3DXVECTOR2* _pPosition,	// Translation
		const D3DXVECTOR3* _pOffset,	// Offset
		const D3DXCOLOR& _color		// Color
	)
	{
		NULL_CHECK_WITH_MSG(m_pDxSprite, "m_pDxSprite is nullptr");

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

		D3DXMATRIX mtResult;
		D3DXMatrixIdentity(&mtResult);

		if (_pScale)
			D3DXMatrixScaling(&mtScl, _pScale->x, _pScale->y, 1.0f);

		D3DXMatrixRotationZ(&mtRot, -fTheta);

		if (_pCenter)
			D3DXMatrixTranslation(&mtRct, _pCenter->x, _pCenter->y, 0.0f);

		//	D3DXMatrixInverse(&mtRctI, NULL, &mtRct);
		if (_pCenter)
			D3DXMatrixTranslation(&mtRctI, -(_pCenter->x), -(_pCenter->y), 0.0f);

		if (_pPosition)
			D3DXMatrixTranslation(&mtTrn, _pPosition->x, _pPosition->y, 0.0f);

		mtW = mtScl * mtRctI * mtRot * mtRct * mtTrn;

		mtResult = mtW * (*Camera->viewMatrix);

		m_pDxSprite->SetTransform(&mtResult);

		m_pDxSprite->Draw(_pTex, _pSrcRect, _pOffset, nullptr, _color);

		D3DXMatrixIdentity(&mtW);
		m_pDxSprite->SetTransform(&mtW);

		m_pDxSprite->End();

		return S_OK;
	}
	HRESULT C_Sprite::Draw(
		LPD3DXTEXTURE9	_pTex,
		const RECT* _pSrcRect,
		const D3DXVECTOR2* _pScale,	// Scaling
		const D3DXVECTOR3* _pRotation,	// Rotation
		const D3DXVECTOR2* _pPosition,	// Translation
		const D3DXVECTOR3* _pOffset,	// Offset
		const D3DXCOLOR& _color		// Color
	)
	{
		NULL_CHECK_WITH_MSG(m_pDxSprite, "m_pDxSprite is nullptr");

		if (!_pRotation)
			return this->Draw(_pTex, _pSrcRect, _pScale, nullptr, 0.0f, _pPosition, _pOffset, _color);

		D3DXVECTOR2 rotation(_pRotation->x, _pRotation->y);
		return this->Draw(_pTex, _pSrcRect, _pScale, &rotation, _pRotation->z, _pPosition, _pOffset, _color);
	}*/

	HRESULT C_Sprite::DrawLine(
		const D3DXVECTOR2* _pVertexList,
		const DWORD& _dwVertexListCount,
		const FLOAT& _fThickness,
		const bool& _bAntialias,
		const D3DCOLOR& _color
	)
	{
		NULL_CHECK_WITH_MSG(m_pDxLine, "m_pDxLine is nullptr");

		m_pDxLine->SetGLLines(false);
		m_pDxLine->SetAntialias(_bAntialias);
		m_pDxLine->SetWidth(_fThickness);

		m_pDxLine->Begin();

		m_pDxLine->Draw(_pVertexList, _dwVertexListCount, _color);

		m_pDxLine->End();

		return S_OK;
	}
	HRESULT C_Sprite::DrawRect(
		const RECT& _rect,
		const FLOAT& _fThickness,
		const bool& _bAntialias,
		const D3DCOLOR& _color
	)
	{
		NULL_CHECK_WITH_MSG(m_pDxLine, "m_pDxLine is nullptr");

		if (m_pDxLine->GetGLLines())
			FAILED_CHECK_RETURN(m_pDxLine->SetGLLines(false));

		if (m_pDxLine->GetAntialias() != _bAntialias)
			FAILED_CHECK_RETURN(m_pDxLine->SetAntialias(_bAntialias));

		if (m_pDxLine->GetWidth() != _fThickness)
			FAILED_CHECK_RETURN(m_pDxLine->SetWidth(_fThickness));

		if (FAILED(m_pDxLine->Begin()))
		{
			FAILED_CHECK_WITH_MSG(m_pDxLine->End(), "m_pDxLine's Begin() and m_pDxLine's End() failed.");
			return E_FAIL;
		}

		D3DXVECTOR2 TempPos[2], LinePos[2];
		const D3DXVECTOR2 LeftTop = D3DXVECTOR2(_rect.left, _rect.top);
		const D3DXVECTOR2 RightBottom = D3DXVECTOR2(_rect.right, _rect.bottom);

		D3DXMATRIX mtV = (*Camera->viewMatrix);

		// ┌─┐
		TempPos[1] = TempPos[0] = LeftTop;
		TempPos[1].x = RightBottom.x;

		D3DXVec2TransformCoord(&LinePos[0], &TempPos[0], &mtV);
		D3DXVec2TransformCoord(&LinePos[1], &TempPos[1], &mtV);

		FAILED_CHECK_RETURN(m_pDxLine->Draw(LinePos, 2, _color));

		// ┐
		// ┘
		TempPos[0] = TempPos[1];
		TempPos[1] = RightBottom;

		D3DXVec2TransformCoord(&LinePos[0], &TempPos[0], &mtV);
		D3DXVec2TransformCoord(&LinePos[1], &TempPos[1], &mtV);

		FAILED_CHECK_RETURN(m_pDxLine->Draw(LinePos, 2, _color));

		// └─┘
		TempPos[0] = TempPos[1];
		TempPos[1].x = LeftTop.x;

		D3DXVec2TransformCoord(&LinePos[0], &TempPos[0], &mtV);
		D3DXVec2TransformCoord(&LinePos[1], &TempPos[1], &mtV);

		FAILED_CHECK_RETURN(m_pDxLine->Draw(LinePos, 2, _color));

		// ┌
		// └
		TempPos[0] = TempPos[1];
		TempPos[1] = LeftTop;

		D3DXVec2TransformCoord(&LinePos[0], &TempPos[0], &mtV);
		D3DXVec2TransformCoord(&LinePos[1], &TempPos[1], &mtV);

		FAILED_CHECK_RETURN(m_pDxLine->Draw(LinePos, 2, _color));

		FAILED_CHECK_RETURN(m_pDxLine->End());

		return S_OK;
	}
	HRESULT C_Sprite::DrawEllipse(
		const FLOAT& _fCenterX,
		const FLOAT& _fCenterY,
		const FLOAT& _fWidth,
		const FLOAT& _fHeight,
		const FLOAT& _fNumSides,
		const FLOAT& _fThickness,
		const D3DCOLOR& _color
	)
	{
		// 출처: https://www.unknowncheats.me/forum/direct3d/467770-directx-9-function-draw-ellipse.html
		float theta = 2.0f * D3DX_PI / _fNumSides;
		float c = cosf(theta); // precalculate the sine and cosine
		float s = sinf(theta);
		float t;

		float x = 1; // we start at angle = 0 
		float y = 0;

		D3DXVECTOR2 TempPos[2], LinePos[2];

		m_pDxLine->SetWidth(_fThickness);
		m_pDxLine->SetAntialias(false);
		m_pDxLine->SetGLLines(true);

		D3DXMATRIX mtV = (*Camera->viewMatrix);

		m_pDxLine->Begin();

		for (int i = 0; i < _fNumSides; ++i)
		{
			// apply radius and offset 1
			float x1 = x * _fWidth + _fCenterX;
			float y1 = y * _fHeight + _fCenterY;

			// apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;

			// apply radius and offset 2
			float x2 = x * _fWidth + _fCenterX;
			float y2 = y * _fHeight + _fCenterY;

			// draw the line
			TempPos[0].x = x1; TempPos[0].y = y1;
			TempPos[1].x = x2; TempPos[1].y = y2;

			D3DXVec2TransformCoord(&LinePos[0], &TempPos[0], &mtV);
			D3DXVec2TransformCoord(&LinePos[1], &TempPos[1], &mtV);

			m_pDxLine->Draw(LinePos, 2, _color);
		}

		m_pDxLine->End();

		return S_OK;
	}

	C_Sprite::C_Sprite()
	{
		m_pDxSprite = nullptr;
		m_pDxLine = nullptr;

		m_pMtPosition = nullptr;
		m_pMtRotation = nullptr;
		m_pMtScale = nullptr;
		m_pMtSRT = nullptr;
		m_pMtResult = nullptr;
	}
	C_Sprite::~C_Sprite()
	{
		Destroy();
	}

	HRESULT C_Sprite::Create()
	{
		FAILED_CHECK_RETURN(D3DXCreateSprite(m_pd3dDevice, &m_pDxSprite));
		FAILED_CHECK_RETURN(D3DXCreateLine(m_pd3dDevice, &m_pDxLine));
		m_pDxLine->SetAntialias(false);
		m_pDxLine->SetWidth(1.0f);

		m_pMtPosition = new D3DXMATRIX();
		m_pMtRotation = new D3DXMATRIX();
		m_pMtScale = new D3DXMATRIX();
		m_pMtSRT = new D3DXMATRIX();
		m_pMtResult = new D3DXMATRIX();

		D3DXMatrixIdentity(m_pMtPosition);
		D3DXMatrixIdentity(m_pMtRotation);
		D3DXMatrixIdentity(m_pMtScale);
		D3DXMatrixIdentity(m_pMtSRT);
		D3DXMatrixIdentity(m_pMtResult);

		return S_OK;
	}
	HRESULT C_Sprite::Create(const LPD3DXSPRITE _pSprite, const LPD3DXLINE _pLine)
	{
		NULL_CHECK_WITH_MSG(_pSprite, "_pSprite is nullptr");
		NULL_CHECK_WITH_MSG(_pLine, "_pLine is nullptr");

		m_pDxSprite = _pSprite;
		m_pDxLine = _pLine;

		m_pMtPosition = new D3DXMATRIX();
		m_pMtRotation = new D3DXMATRIX();
		m_pMtScale = new D3DXMATRIX();
		m_pMtSRT = new D3DXMATRIX();
		m_pMtResult = new D3DXMATRIX();

		D3DXMatrixIdentity(m_pMtPosition);
		D3DXMatrixIdentity(m_pMtRotation);
		D3DXMatrixIdentity(m_pMtScale);
		D3DXMatrixIdentity(m_pMtSRT);
		D3DXMatrixIdentity(m_pMtResult);

		return S_OK;
	}
	void C_Sprite::Destroy()
	{
		SAFE_DELETE(m_pMtResult);
		SAFE_DELETE(m_pMtSRT);
		SAFE_DELETE(m_pMtScale);
		SAFE_DELETE(m_pMtRotation);
		SAFE_DELETE(m_pMtPosition);
	}
}