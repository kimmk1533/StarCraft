#include "stdafx_Core.h"
#include "Physics.h"

#include "Bounds.h"
#include "QuadTree.h"

namespace CoreEngine
{
	C_QuadTree<ICollision>* Physics::m_pQuadTree = nullptr;

	bool Physics::CollisionCheck(const D3DXVECTOR2& _left, const S_Bounds& _right)
	{
		if (_right.c_min.x <= _left.x &&
			_right.c_max.x >= _left.x &&
			_right.c_min.y <= _left.y &&
			_right.c_max.y >= _left.y)
			return true;

		return false;
	}
	bool Physics::CollisionCheck(const S_Bounds& _left, const D3DXVECTOR2& _right)
	{
		return Physics::CollisionCheck(_right, _left);
	}
	bool Physics::CollisionCheck(const D3DXVECTOR3& _left, const S_Bounds& _right)
	{
		if (_right.c_min.x <= _left.x &&
			_right.c_max.x >= _left.x &&
			_right.c_min.y <= _left.y &&
			_right.c_max.y >= _left.y &&
			_right.c_min.z <= _left.z &&
			_right.c_max.z >= _left.z)
			return true;

		return false;
	}
	bool Physics::CollisionCheck(const S_Bounds& _left, const D3DXVECTOR3& _right)
	{
		return Physics::CollisionCheck(_right, _left);
	}
	bool Physics::CollisionCheck(const S_Bounds& _left, const S_Bounds& _right, const bool& _checkIncludeOther)
	{
		if (_checkIncludeOther)
		{
			if (_left.c_max.x > _right.c_max.x)
			{
				if (_left.c_min.x < _right.c_min.x &&
					_left.c_max.x > _right.c_max.x &&
					_left.c_min.y < _right.c_min.y &&
					_left.c_max.y > _right.c_max.y)
					return true;
			}
			else
			{
				if (_right.c_min.x < _left.c_min.x &&
					_right.c_max.x > _left.c_max.x &&
					_right.c_min.y < _left.c_min.y &&
					_right.c_max.y > _left.c_max.y)
					return true;
			}
		}
		else
		{
			if (_left.c_min.x < _right.c_max.x &&
				_left.c_max.x > _right.c_min.x &&
				_left.c_min.y < _right.c_max.y &&
				_left.c_max.y > _right.c_min.y)
				return true;
		}

		return false;
	}

	void Physics::AddCollision(ICollision* _obj)
	{
		m_pQuadTree->AddCollision(_obj);
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

	HRESULT Physics::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(m_pQuadTree);

		return S_OK;
	}
	HRESULT Physics::Render()
	{
		if (FAILED(m_pQuadTree->Debug_Render()))
			return E_FAIL;

		return S_OK;
	}
}