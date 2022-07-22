#pragma once
#include "Collision.h"
#include "Bounds.h"

namespace CoreEngine
{
	class C_Collider : public IFrameWork, public ICollision
	{
	protected:
		S_Bounds* m_pBounds;

	public:
		READONLY_PROPERTY(S_Bounds*, bounds);
		GET(bounds) { return m_pBounds; }
		const GET(bounds) const { return m_pBounds; }

	public:
		C_Collider();
		virtual ~C_Collider();

	public:
		virtual HRESULT Create() override;
		virtual void	Destroy() override;

	protected:
		virtual void OnCollisionBegin(const C_Collider& _other) override;
		virtual void OnCollisionStay(const C_Collider& _other) override;
		virtual void OnCollisionEnd(const C_Collider& _other) override;
		virtual D3DXVECTOR3 GetPosition() override;
		virtual C_Collider GetCollider() override;
		virtual S_Bounds GetBounds() override;

	};
}