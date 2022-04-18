#pragma once
#include "UnitInfo.h"

class CUnit : CFrameWork
{
protected:
	S_UnitInfo* m_Info;

	CAnimation* m_Animation;

public:
	CUnit();
	virtual ~CUnit();

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override = 0;
	virtual HRESULT	Render(CLcSprite* _drawer) override = 0;
	virtual void	Destroy() override;

	virtual CLcTexture* GetTexture() const = 0;
};