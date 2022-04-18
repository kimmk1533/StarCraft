#pragma once
#include "UnitInfo.h"
#include "Animator.h"

class CUnit : public CFrameWork
{
protected:
	S_UnitInfo* m_Info;

	CAnimator* m_Animator;

public:
	CUnit();
	virtual ~CUnit();

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override = 0;
	virtual HRESULT	Render() override = 0;
	virtual void	Destroy() override;

	virtual CLcTexture* GetTexture() const = 0;
};