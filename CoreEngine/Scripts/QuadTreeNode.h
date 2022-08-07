#pragma once
#include "Physics.h"
#include "BoxCollider.h"

//#define USE_SHARED_PTR

namespace CoreEngine
{
	enum class E_NodeType : char
	{
		None = 0,

		LeftTop = 1,
		RightTop = 2,
		LeftBottom = 3,
		RightBottom = 4,
		Root = 5,

		Max
	};
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
		WORD m_Depth;
		WORD m_MaxDepth;
		E_NodeType m_Type;
		C_BoxCollider* m_pBoxCollider;
		PtrType m_pParentNode;
		PtrType m_pRootNode;
		PtrType m_pChildNodes[Quad];
		std::vector<T*>* m_pObjList;

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
		static PtrType BuildRootTree(const D3DXVECTOR3& _center, const D3DXVECTOR3& _size, const WORD& _depth = Quad)
		{
			// 최상단 노드 생성
#ifdef USE_SHARED_PTR
			PtrType root = std::make_shared<QuadTreeNode>();
#else
			PtrType root = new QuadTreeNode();
#endif // USE_SHARED_PTR
			if (FAILED(root->Create()))
				return nullptr;
			root->m_Type = E_NodeType::Root;
			root->m_pBoxCollider->bounds->center = _center;
			root->m_pBoxCollider->bounds->size = _size;
			root->m_pRootNode = root;
			//root->m_Depth = ;
			root->m_MaxDepth = _depth;

			// 자식 노드 생성에 사용할 변수
			D3DXVECTOR3 offset;
			D3DXVECTOR3 childCenter = _center;
			D3DXVECTOR3 childSize = _size * 0.5f;

#ifdef DEBUG_QuadTree_PRINT
			std::cout << "[ depth " << 0 << " ]\n";
#endif // DEBUG_QuadTree_PRINT

			// 자식 노드 생성
			for (WORD i = 0; i < Quad; ++i)
			{
#ifdef DEBUG_QuadTree_PRINT
				switch (static_cast<E_NodeType>(i))
				{
				case E_NodeType::LeftTop:
					std::cout << " Left  Top\n";
					break;
				case E_NodeType::RightTop:
					std::cout << " Right  Top\n";
					break;
				case E_NodeType::LeftBottom:
					std::cout << " Left  Bottom\n";
					break;
				case E_NodeType::RightBottom:
					std::cout << " Right  Bottom\n";
					break;
				}
#endif // DEBUG_QuadTree_PRINT

				offset.x = ((i & 1) ? childSize.x : -childSize.x);
				offset.y = ((i & 2) ? childSize.y : -childSize.y);

				offset *= 0.5f;

#ifdef DEBUG_QuadTree_PRINT
				std::cout << "  offset.x = " << offset.x << "\n";
				std::cout << "  offset.y = " << offset.y << "\n\n";
#endif // DEBUG_QuadTree_PRINT

				childCenter.x = offset.x + _center.x;
				childCenter.y = offset.y + _center.y;

#ifdef DEBUG_QuadTree_PRINT
				std::cout << "  childCenter.x = " << childCenter.x << "\n";
				std::cout << "  childCenter.y = " << childCenter.y << "\n\n";
#endif // DEBUG_QuadTree_PRINT

				root->AddChildNode(root->BuildChildTree(root, childCenter, childSize, static_cast<E_NodeType>(i), _depth));
			}

			return root;
		}
		bool AddObj(T* _obj);

	private:
		PtrType BuildChildTree(const PtrType _root, const D3DXVECTOR3& _center, const D3DXVECTOR3& _size, const E_NodeType& _type, const WORD& _depth)
		{
			if (_depth <= 0)
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
			node->m_pRootNode = _root;
			//node->m_Depth = 
			node->m_MaxDepth = _root->m_MaxDepth;

			// 자식 노드 생성에 사용할 변수
			D3DXVECTOR3 offset;
			D3DXVECTOR3 childCenter;
			D3DXVECTOR3 childSize = _size * 0.5f;

#ifdef DEBUG_QuadTree_PRINT
			std::cout << "[ depth " << Quad - _depth << " ]\n";
#endif // DEBUG_QuadTree_PRINT

			// 자식 노드 생성
			for (WORD i = 0; i < Quad; ++i)
			{
#ifdef DEBUG_QuadTree_PRINT
				switch (static_cast<E_NodeType>(i))
				{
				case E_NodeType::LeftTop:
					std::cout << " Left  Top\n";
					break;
				case E_NodeType::RightTop:
					std::cout << " Right  Top\n";
					break;
				case E_NodeType::LeftBottom:
					std::cout << " Left  Bottom\n";
					break;
				case E_NodeType::RightBottom:
					std::cout << " Right  Bottom\n";
					break;
				}
#endif // DEBUG_QuadTree_PRINT

				offset.x = ((i & 1) ? childSize.x : -childSize.x);
				offset.y = ((i & 2) ? childSize.y : -childSize.y);

				offset *= 0.5f;

#ifdef DEBUG_QuadTree_PRINT
				std::cout << "  offset.x = " << offset.x << "\n";
				std::cout << "  offset.y = " << offset.y << "\n\n";
#endif // DEBUG_QuadTree_PRINT

				childCenter.x = offset.x + _center.x;
				childCenter.y = offset.y + _center.y;

#ifdef DEBUG_QuadTree_PRINT
				std::cout << "  childCenter.x = " << childCenter.x << "\n";
				std::cout << "  childCenter.y = " << childCenter.y << "\n\n";
#endif // DEBUG_QuadTree_PRINT

				PtrType child = node->BuildChildTree(_root, childCenter, childSize, static_cast<E_NodeType>(i), _depth - 1);
				if (child == nullptr)
					continue;

				node->AddChildNode(child);
			}

			return node;
		}

		void AddChildNode(const PtrType _node)
		{
			for (WORD i = 0; i < Quad; ++i)
			{
				if (m_pChildNodes[i] == nullptr)
				{
					m_pChildNodes[i] = _node;
#ifdef USE_SHARED_PTR
					_node->m_pParentNode = std::make_shared<QuadTreeNode>(); // 수정 필요
#else
					_node->m_pParentNode = this;
#endif // USE_SHARED_PTR

					return;
				}
			}
		}
		E_NodeType FindType(const D3DXVECTOR3& _pos);
		bool CheckObj(const T* _obj);
		bool AddObj(T* _obj, const S_Bounds& _bounds);

	public:
		C_QuadTreeNode();
		~C_QuadTreeNode();

	public:
		HRESULT Create() override;
		void	Destroy() override;

	public:
		HRESULT Update(const FLOAT& _deltaTime) override;
		HRESULT Debug_Render(size_t _depth = 0);

	};

	template<class T>
	E_NodeType C_QuadTreeNode<T>::FindType(const D3DXVECTOR3& _pos)
	{
		D3DXVECTOR3 center = m_pBoxCollider->bounds->center;

		char bit = 0;

		if (center.x < _pos.x) bit &= 1;
		if (center.y < _pos.y) bit &= 2;

		return static_cast<E_NodeType>(bit);
	}
	template<class T>
	inline bool C_QuadTreeNode<T>::AddObj(T* _obj)
	{
		static_assert(std::is_base_of_v<ICollision, T>, "T of QuadTreeNode<T> is not base of ICollision");

		ICollision* obj = static_cast<ICollision*>(const_cast<T*>(_obj));
		S_Bounds bounds = obj->GetBounds();

		if (!Physics::CollisionCheck((*m_pBoxCollider->bounds), bounds))
			return false;

		bool flag = false;

		for (size_t i = 0; i < Quad; ++i)
		{
			if (m_pChildNodes[i] == nullptr)
				continue;

			if (m_pChildNodes[i]->AddObj(_obj, bounds))
				flag = true;
		}

		if (!flag && !CheckObj(_obj))
			m_pObjList->push_back(_obj);

		return true;
	}
	template<class T>
	inline bool C_QuadTreeNode<T>::CheckObj(const T* _obj)
	{
		return contains(*m_pObjList, _obj);
	}

	template<class T>
	inline bool C_QuadTreeNode<T>::AddObj(T* _obj, const S_Bounds& _bounds)
	{
		if (!Physics::CollisionCheck((*m_pBoxCollider->bounds), _bounds))
			return false;

		bool flag = false;

		for (size_t i = 0; i < Quad; ++i)
		{
			if (m_pChildNodes[i] == nullptr)
				continue;

			if (m_pChildNodes[i]->AddObj(_obj, _bounds))
				flag = true;
		}

		if (!flag && !CheckObj(_obj))
			m_pObjList->push_back(_obj);

		return true;
	}

	template <class T>
	C_QuadTreeNode<T>::C_QuadTreeNode()
	{
		m_Type = E_NodeType::None;
		m_MaxDepth = m_Depth = -1;
		m_pBoxCollider = nullptr;
		m_pParentNode = nullptr;
		m_pRootNode = nullptr;
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
		m_pObjList = new std::vector<T*>();

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
		static_assert(std::is_base_of_v<ICollision, T>, "T of QuadTreeNode<T> is not base of ICollision");

		for (size_t i = 0; i < Quad; ++i)
		{
			if (nullptr != m_pChildNodes[i])
				SAFE_UPDATE(m_pChildNodes[i]);
		}

		size_t size = m_pObjList->size();
		for (size_t i = 0; i < size; ++i)
		{
			ICollision* obj = static_cast<ICollision*>((*m_pObjList)[i]);

			// 자신에게 속한 오브젝트가 빠져나갔는 지 확인.
			// 빠져나갔다면 자신의 부모 노드에게 해당 오브젝트 추가
			// 자신은 오브젝트 제거

			// 다른 섹터와 겹친 경우
			if (!Physics::CollisionCheck((*m_pBoxCollider->bounds), obj->GetBounds(), true))
			{
				m_pRootNode->AddObj((*m_pObjList)[i]);

				// 현재 섹터에서 완전히 나간 경우
				if (!Physics::CollisionCheck((*m_pBoxCollider->bounds), obj->GetBounds()))
					m_pObjList->erase(m_pObjList->begin() + i);
			}
		}

		return S_OK;
	}
	template<class T>
	inline HRESULT C_QuadTreeNode<T>::Debug_Render(size_t _depth)
	{
#ifdef DEBUG_QuadTree_RENDER
		if (_depth > m_MaxDepth)
			return S_OK;

		for (size_t i = 0; i < Quad; i++)
		{
			if (m_pChildNodes[i] != nullptr)
				m_pChildNodes[i]->Debug_Render(_depth + 1);
		}

		S_Bounds* bounds = m_pBoxCollider->bounds;
		RECT rc;
		rc.left = lroundf(-bounds->size.x * 0.5f);
		rc.bottom = lroundf(-bounds->size.y * 0.5f);
		rc.right = bounds->size.x * 0.5f - 1;
		rc.top = bounds->size.y * 0.5f - 1;

		Sprite->SetTranslation(bounds->center);
		Sprite->SetRotation(nullptr);
		Sprite->SetScale(nullptr);

		Sprite->DrawRect(rc, 1.0f, false, D3DCOLOR_XRGB(255, 255, 255));

		if (m_pObjList->size() > 0)
		{
			++rc.left; --rc.top;
			--rc.right; ++rc.bottom;

			Sprite->DrawRect(rc, 1.0f, false, D3DCOLOR_XRGB(255, 0, 0));
		}
#endif // DEBUG_QuadTree_RENDER

		return S_OK;
	}
}