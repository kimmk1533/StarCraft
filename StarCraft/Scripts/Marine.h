#pragma once
#include "Unit.h"

class C_Marine : public C_Unit
{
public:
	C_Marine();
	virtual ~C_Marine() override;

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override;
	virtual HRESULT	Render() override;
	virtual void	Destroy() override;
	virtual C_Texture* GetTexture() const override;
};