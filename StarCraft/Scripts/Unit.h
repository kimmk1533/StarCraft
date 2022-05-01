#pragma once
#include "UnitInfo.h"
//#include <Animator.h>
#include "..\..\CoreEngine\Scripts\Animator.h"

class C_Unit : public C_FrameWork
{
protected:
	S_UnitInfo* m_Info;

protected:
	D3DXVECTOR2 m_Position;
	D3DXVECTOR2 m_TargetPos;

	C_Animator* m_Animator;

public:
	C_Unit();
	virtual ~C_Unit();

	virtual HRESULT	Create() override;
	virtual HRESULT	Update(const float _deltaTime) override = 0;
	virtual HRESULT	Render() override = 0;
	virtual void	Destroy() override;

public:
	virtual C_Texture* GetTexture() const = 0;

};