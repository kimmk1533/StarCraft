#pragma once
#include "QuadTreeNode.h"

namespace CoreEngine
{
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
		HRESULT Create() override;
		HRESULT Create(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size);
		void	Destroy() override;

	public:
		HRESULT Update(const FLOAT& _deltaTime) override;

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

	template <class T>
	HRESULT C_QuadTree<T>::Create()
	{
		RECT rc = C_Engine::GetScreenRect();
		FLOAT width = static_cast<FLOAT>(rc.right - rc.left);
		FLOAT height = static_cast<FLOAT>(rc.bottom - rc.top);
		D3DXVECTOR3 size(width, height, 0.0f);
		D3DXVECTOR3 center = size * 0.5f;

		m_pRootNode = QuadTreeNode::BuildTree(center, size);

		return S_OK;
	}
	template <class T>
	HRESULT C_QuadTree<T>::Create(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size)
	{
		this->m_pRootNode = C_QuadTreeNode<T>::BuildTree(_center, _size);

		return S_OK;
	}
	template <class T>
	void	C_QuadTree<T>::Destroy()
	{
		m_pRootNode = nullptr;
	}

	template<class T>
	HRESULT C_QuadTree<T>::Update(const FLOAT& _deltaTime)
	{
		SAFE_UPDATE(this->m_pRootNode);

		return S_OK;
	}

}