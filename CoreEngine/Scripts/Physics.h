#pragma once
#include "Collision.h"
#include "QuadTree.h"

namespace CoreEngine
{
	class Physics final
	{
	public:
		static bool AABB_Collision(const C_Collider& _left, const C_Collider& _right);

	private:
		static C_QuadTree<ICollision>* m_pQuadTree;

	private:
		Physics() = delete;
		~Physics() = delete;

	public:
		static HRESULT Create();
		static void	Destroy();

	public:
		static HRESULT Update(const FLOAT& _deltaTime);

	};
}