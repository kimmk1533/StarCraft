#include "stdafx_Core.h"
#include "Physics.h"

#include "Collider.h"
#include "Bounds.h"

namespace CoreEngine
{
	C_QuadTree<ICollision>* Physics::m_pQuadTree = nullptr;

	bool Physics::AABB_Collision(const C_Collider& _left, const C_Collider& _right)
	{
		S_Bounds* left = _left.bounds;
		S_Bounds* right = _right.bounds;

		if (left->min.x < right->max.x &&
			left->max.x > right->min.x &&
			left->min.y < right->max.y &&
			left->max.y > right->min.y)
			return true;

		return false;
	}

	HRESULT Physics::Update(const FLOAT& _deltaTime)
	{
		//SAFE_UPDATE(m_pQuadTree);

		return S_OK;
	}
	HRESULT Physics::Create()
	{
		m_pQuadTree = new C_QuadTree<ICollision>();
		SAFE_CREATE(m_pQuadTree);

		return S_OK;
	}
	void Physics::Destroy()
	{
		SAFE_DELETE(m_pQuadTree);
	}
}