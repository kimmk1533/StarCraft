#pragma once
#include "QuadTreeNode.h"

namespace CoreEngine
{
	// 참고: https://blog.naver.com/hermet/57456541

	template <class T>
	class C_QuadTree final : public IFrameWork, public IUpdatable
	{
	private:
		using QuadTreeNode = C_QuadTreeNode<T>;
#ifdef USE_SHARED_PTR
		using PtrType = std::shared_ptr<QuadTreeNode>;
#else
		using PtrType = QuadTreeNode*;
#endif // USE_SHARED_PTR

	private:		
		PtrType m_pRootNode;

	public:
		C_QuadTree();
		~C_QuadTree();

	public:
		void AddCollision(T* _obj);

	public:
		HRESULT Create() override;
		HRESULT Create(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size, const WORD& _depth = 1);
		void	Destroy() override;

	public:
		HRESULT Update(const FLOAT& _deltaTime) override;
		HRESULT Debug_Render();

	};

	template <class T>
	C_QuadTree<T>::C_QuadTree()
	{
		m_pRootNode = nullptr;
	}
	template <class T>
	C_QuadTree<T>::~C_QuadTree()
	{
		Destroy();
	}

	template<class T>
	inline void C_QuadTree<T>::AddCollision(T* _obj)
	{
		static_assert(std::is_base_of_v<ICollision, T>, "T of QuadTree<T> is not base of ICollision");

		m_pRootNode->AddObj(_obj);
	}

	template <class T>
	HRESULT C_QuadTree<T>::Create()
	{
		RECT rc = C_Engine::GetScreenRect();
		FLOAT width = static_cast<FLOAT>(rc.right - rc.left);
		FLOAT height = static_cast<FLOAT>(rc.bottom - rc.top);
		D3DXVECTOR3 size(width, height, 0.0f);
		D3DXVECTOR3 center = size * 0.5f;

		m_pRootNode = QuadTreeNode::BuildRootTree(center, size);

		return S_OK;
	}
	template <class T>
	HRESULT C_QuadTree<T>::Create(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size, const WORD& _depth)
	{
		this->m_pRootNode = C_QuadTreeNode<T>::BuildRootTree(_center, _size, _depth);

		return S_OK;
	}
	template <class T>
	void	C_QuadTree<T>::Destroy()
	{
		SAFE_DELETE(m_pRootNode);
	}

	template<class T>
	HRESULT C_QuadTree<T>::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(this->m_pRootNode);

		return S_OK;
	}
	template<class T>
	inline HRESULT C_QuadTree<T>::Debug_Render()
	{
		if (FAILED(this->m_pRootNode->Debug_Render()))
			return E_FAIL;

		return S_OK;
	}
}