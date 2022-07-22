#pragma once
#include "Collision.h"

namespace CoreEngine
{
	struct S_Bounds;

	template <class T>
	class C_QuadTree;
}

namespace CoreEngine
{
	class Physics final
	{
	public:
		static bool CollisionCheck(const D3DXVECTOR2& _left, const S_Bounds& _right); // Point vs Box
		static bool CollisionCheck(const S_Bounds& _left, const D3DXVECTOR2& _right); // Box vs Point
		static bool CollisionCheck(const D3DXVECTOR3& _left, const S_Bounds& _right); // Point vs Box
		static bool CollisionCheck(const S_Bounds& _left, const D3DXVECTOR3& _right); // Box vs Point
		static bool CollisionCheck(const S_Bounds& _left, const S_Bounds& _right, const bool& _checkIncludeOther = false); // Box vs Box

	private:
		static C_QuadTree<ICollision>* m_pQuadTree;

	public:
		static void AddCollision(ICollision* _obj);

	private:
		Physics() = delete;
		~Physics() = delete;

	public:
		static HRESULT Create();
		static void	Destroy();

	public:
		static HRESULT Update(const FLOAT& _deltaTime);
		static HRESULT Render();

	};
}