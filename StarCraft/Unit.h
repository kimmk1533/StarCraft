#pragma once
#include "UnitInfo.h"
#include "Animator.h"

class CUnit : public CFrameWork
{
protected:
	S_UnitInfo* m_Info;

protected:
	D3DXVECTOR2 m_Position;
	D3DXVECTOR2 m_TargetPos;

	CAnimator* m_Animator;

public:
	CUnit();
	virtual ~CUnit();

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override = 0;
	virtual HRESULT	Render() override = 0;
	virtual void	Destroy() override;

public:
	virtual CLcTexture* GetTexture() const = 0;

};