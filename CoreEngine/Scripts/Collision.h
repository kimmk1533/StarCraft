#pragma once

namespace CoreEngine
{
	class C_Collider;
	struct S_Bounds;
}

namespace CoreEngine
{
	__interface ICollisionBegin
	{
		virtual void OnCollisionBegin(const C_Collider& _other) = 0;
	};
	__interface ICollisionStay
	{
		virtual void OnCollisionStay(const C_Collider& _other) = 0;
	};
	__interface ICollisionEnd
	{
		virtual void OnCollisionEnd(const C_Collider& _other) = 0;
	};

	__interface ICollision : public ICollisionBegin, public ICollisionStay, public ICollisionEnd
	{
		virtual D3DXVECTOR3 GetPosition() = 0;
		virtual C_Collider GetCollider() = 0;
		virtual S_Bounds GetBounds() = 0;
	};
}