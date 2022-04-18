#pragma once
#include "Unit.h"

class CMarine : public CUnit
{
private:
	std::shared_ptr<CLcTexture*> m_pTex; // 매니저에서 관리하는 방식으로 변경해야함.

public:
	CMarine();
	virtual ~CMarine() override;

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	Destroy() override;
	virtual CLcTexture* GetTexture() const override;
};