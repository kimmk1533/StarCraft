#pragma once
#include "Animation.h"

enum class E_AnimState
{
	Init,
	Death,

	Idle,
	Other,

	GroundAttackInit,
	AirAttackInit,
	GroundAttackRepeat,
	AirAttackRepeat,
	CastSpell,
	GroundAttackToIdle,
	AirAttackToIdle,

	Walking,
	WalkingToIdle,

	Built,
	AlmostBuilt,
	Landing,
	LiftOff,

	IsWorking,
	WorkingToIdle,

	Burrow,
	UnBurrow,

	Max
};

class CAnimator : CFrameWork
{
private:
	D3DXVECTOR2 m_Position;
	D3DXVECTOR3 m_Rotation; // x, y는 회전 중심점. z는 회전 각도(degree)
	D3DXVECTOR2 m_Scale;
	D3DXCOLOR	m_Color;
	
public:
	CAnimator();
	virtual ~CAnimator();

	HRESULT	Create() override;
	HRESULT	Update(const float _deltaTime) override;
	HRESULT Render(CLcSprite* _drawer) override;
	void	Destroy() override;

protected:
	std::map<E_AnimState, CAnimation> m_AnimMap;

public:
	D3DXVECTOR2 Position() { return m_Position; }
	void Position(float _x, float _y);
	void Position(D3DXVECTOR2 _pos);

	D3DXVECTOR3 Rotation() { return m_Rotation; }
	void Rotation(float _x, float _y, float _degree);
	void Rotation(D3DXVECTOR3 _rot);

	D3DXVECTOR2 Scale() { return m_Scale; }
	void Scale(float _x, float _y);
	void Scale(D3DXVECTOR2 _scale);

	D3DXCOLOR Color() { return m_Color; }
	void Color(float _r, float _g = 1.0f, float _b = 1.0f, float _a = 1.0f);
	void Color(D3DXCOLOR _color);

	void AddFrame(std::shared_ptr<CLcTexture*>& _texture, const RECT _size, float _time);
};