#pragma once
#include "BoxCollider.h"

//#define USE_SHARED_PTR

namespace CoreEngine
{
	enum class E_NodeType : char;
}

namespace CoreEngine
{
	// 참고: https://blog.naver.com/hermet/57456541
	template <class T>
	class C_QuadTreeNode final : public IFrameWork, public IUpdatable
	{
	private:
		static const WORD Quad = 4;
		using QuadTreeNode = C_QuadTreeNode<T>;
#ifdef USE_SHARED_PTR
		using PtrType = std::shared_ptr<QuadTreeNode>;
#else
		using PtrType = QuadTreeNode*;
#endif // USE_SHARED_PTR

	private:
		E_NodeType m_Type;
		C_BoxCollider* m_pBoxCollider;
		PtrType m_pParentNode;
		PtrType m_pChildNodes[Quad];
		std::vector<T>* m_pObjList;

	public:
		PROPERTY(E_NodeType, type);
		GET(type) { return m_Type; }
		SET(type)
		{
			for (WORD i = 0; i < Quad; ++i)
			{
				std::shared_ptr<QuadTreeNode> node = m_pParentNode->m_pChildNodes[i];
				if (node.get() == this) continue;

				if (m_pParentNode->m_pChildNodes[i]->m_Type == _value)
				{
					ErrorMessageBox("QuadTreeNode Type is same.");
					return;
				}
			}

			m_Type = _value;
		}

	public:
		static PtrType BuildTree(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size)
		{
			// 최상단 노드 생성
#ifdef USE_SHARED_PTR
			PtrType node = std::make_shared<QuadTreeNode>();
#else
			PtrType node = new QuadTreeNode();
#endif // USE_SHARED_PTR
			if (FAILED(node->Create()))
				return nullptr;
			node->m_Type = E_NodeType::Root;
			node->m_pBoxCollider->bounds->center = _center;
			node->m_pBoxCollider->bounds->size = _size;

			// 자식 노드 생성에 사용할 변수
			D3DXVECTOR3 offset;
			D3DXVECTOR3 childCenter;
			D3DXVECTOR3 childSize = _size * 0.5f;

			// 자식 노드 생성
			for (WORD i = 0; i < Quad; ++i)
			{
				offset.x = ((i | 1) ? -childSize.x : childSize.x);
				offset.y = ((i | 2) ? childSize.y : -childSize.y);

				childCenter.x = offset.x + _center.x;
				childCenter.y = offset.y + _center.y;

				node->AddChildNode(node->BuildTree(childCenter, childSize, static_cast<E_NodeType>(i), 1));
			}

			return node;
		}

	private:
		PtrType BuildTree(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size, const E_NodeType& _type, const WORD& _depth)
		{
			if (_depth >= Quad)
				return nullptr;

			// 노드 생성
#ifdef USE_SHARED_PTR
			PtrType node = std::make_shared<QuadTreeNode>();
#else
			PtrType node = new QuadTreeNode();
#endif // USE_SHARED_PTR
			if (FAILED(node->Create()))
				return nullptr;
			node->m_Type = _type;
			node->m_pBoxCollider->bounds->center = _center;
			node->m_pBoxCollider->bounds->size = _size;

			// 자식 노드 생성에 사용할 변수
			D3DXVECTOR3 offset;
			D3DXVECTOR3 childCenter;
			D3DXVECTOR3 childSize = _size * 0.5f;

			// 자식 노드 생성
			for (WORD i = 0; i < Quad; ++i)
			{
				offset.x = ((i | 1) ? -childSize.x : childSize.x);
				offset.y = ((i | 2) ? childSize.y : -childSize.y);

				childCenter.x = offset.x + _center.x;
				childCenter.y = offset.y + _center.y;

				PtrType child = node->BuildTree(childCenter, childSize, static_cast<E_NodeType>(i), _depth + 1);
				if (child == nullptr)
					continue;

				node->AddChildNode(child);
			}

			return node;
		}
#ifdef USE_SHARED_PTR
		void AddChildNode(const std::shared_ptr<QuadTreeNode> _node)
#else
		void AddChildNode(const PtrType _node)
#endif // USE_SHARED_PTR
		{
			for (WORD i = 0; i < Quad; ++i)
			{
				if (m_pChildNodes[i] == nullptr)
				{
					m_pChildNodes[i] = _node;
#ifdef USE_SHARED_PTR
					_node->m_pParentNode = std::make_shared<QuadTreeNode>();
#else
					_node->m_pParentNode = this;
#endif // USE_SHARED_PTR

					return;
				}
			}
		}
		E_NodeType FindType(const D3DXVECTOR3& _pos);
		bool CheckObj(const T& _obj);

	public:
		C_QuadTreeNode();
		~C_QuadTreeNode();

	public:
		HRESULT Create() override;
		void	Destroy() override;

	public:
		HRESULT Update(const FLOAT& _deltaTime) override;

	};

	enum class E_NodeType : char
	{
		Root = -2,
		None = -1,

		LeftTop = 0,
		RightTop = 1,
		LeftBottom = 2,
		RightBottom = 3,

		Max
	};

	template<class T>
	E_NodeType C_QuadTreeNode<T>::FindType(const D3DXVECTOR3& _pos)
	{
		D3DXVECTOR3 center = m_pBoxCollider->bounds->center;

		char bit = 0;

		if (center.x < _pos.x) bit |= 1;
		if (center.y < _pos.y) bit |= 2;

		return static_cast<E_NodeType>(bit);
	}

	template<class T>
	inline bool C_QuadTreeNode<T>::CheckObj(const T& _obj)
	{
		//return _obj;
		return true;
	}

	template <class T>
	C_QuadTreeNode<T>::C_QuadTreeNode()
	{
		m_Type = E_NodeType::None;
		m_pBoxCollider = nullptr;
		m_pParentNode = nullptr;
		for (WORD i = 0; i < Quad; ++i)
		{
			m_pChildNodes[i] = nullptr;
		}
		m_pObjList = nullptr;
	}
	template <class T>
	C_QuadTreeNode<T>::~C_QuadTreeNode()
	{
		Destroy();
	}

	template<class T>
	HRESULT C_QuadTreeNode<T>::Create()
	{
		m_pBoxCollider = new C_BoxCollider();
		SAFE_CREATE(m_pBoxCollider);
		m_pObjList = new std::vector<T>();

		return S_OK;
	}
	template<class T>
	void C_QuadTreeNode<T>::Destroy()
	{
		SAFE_DELETE(m_pObjList);
		SAFE_DELETE(m_pBoxCollider);
		for (size_t i = 0; i < Quad; ++i)
		{
			m_pChildNodes[i] = nullptr;
		}
		m_pParentNode = nullptr;
	}

	template<class T>
	HRESULT C_QuadTreeNode<T>::Update(const FLOAT& _deltaTime)
	{
		for (size_t i = 0; i < m_pObjList->size(); ++i)
		{
			//(*m_pObjList)[i]
		}

		return S_OK;
	}
}