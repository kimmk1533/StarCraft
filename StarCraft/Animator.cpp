#include "stdafx.h"
#include "Animator.h"

CAnimator::CAnimator()
{
	m_Position = D3DXVECTOR2(0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR2(1.0f, 1.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

CAnimator::~CAnimator()
{
}

HRESULT CAnimator::Create()
{
	return E_NOTIMPL;
}

HRESULT CAnimator::Update(const float _deltaTime)
{
	return E_NOTIMPL;
}

HRESULT CAnimator::Render(CLcSprite* _drawer)
{
	return E_NOTIMPL;
}

void CAnimator::Destroy()
{
}

void CAnimator::Position(float _x, float _y)
{
	this->Position(D3DXVECTOR2(_x, _y));
}
void CAnimator::Position(D3DXVECTOR2 _pos)
{
	m_Position = _pos;
}

void CAnimator::Rotation(float _x, float _y, float _degree)
{
	this->Rotation(D3DXVECTOR3(_x, _y, _degree));
}
void CAnimator::Rotation(D3DXVECTOR3 _rot)
{
	m_Rotation = _rot;
}

void CAnimator::Scale(float _x, float _y)
{
	this->Scale(D3DXVECTOR2(_x, _y));
}
void CAnimator::Scale(D3DXVECTOR2 _scale)
{
	m_Scale = _scale;
}

void CAnimator::Color(float _r, float _g, float _b, float _a)
{
	this->Color(D3DXCOLOR(_r, _g, _b, _a));
}
void CAnimator::Color(D3DXCOLOR _color)
{
	m_Color = _color;
}
