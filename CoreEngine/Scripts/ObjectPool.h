#pragma once

namespace CoreEngine
{
	template<class T>
	class C_ObjectPool final : public IFrameWork
	{
	private:
		int m_Capacity;
		std::queue<std::shared_ptr<T>>* m_pQueue;
		std::vector<std::shared_ptr<T>>* m_pSpawnedObjList;

	private:
		void ExpandPool()
		{
			for (int i = 0; i < m_Capacity; ++i)
			{
				std::shared_ptr<T> item = std::make_shared<T>();

				if (std::is_base_of<IFrameWork, T>::value)
				{
					if (FAILED((*item).Create()))
					{
						std::cout << "ObjectPool ExpandPool: Object Create Error\n";
						return;
					}
				}

				m_pQueue->push(item);
			}
			m_Capacity *= 2;
		}

	public:
		C_ObjectPool(int _capacity = 10)
		{
			m_Capacity = _capacity;
			m_pQueue = nullptr;
			m_pSpawnedObjList = nullptr;
		}
		virtual ~C_ObjectPool()
		{
			Destroy();
		}

	public:
		HRESULT Create() override
		{
			m_pQueue = new std::queue<std::shared_ptr<T>>();
			for (int i = 0; i < m_Capacity; ++i)
			{
				std::shared_ptr<T> item = std::make_shared<T>();

				if (std::is_base_of<IFrameWork, T>::value)
				{
					if (FAILED((*item).Create()))
					{
						std::cout << "ObjectPool Create: Object Create Error\n";
						return E_FAIL;
					}
				}

				m_pQueue->push(item);
			}

			m_pSpawnedObjList = new std::vector<std::shared_ptr<T>>();

			return S_OK;
		}
		void	Destroy() override
		{
			for (unsigned int i = 0; i < m_pQueue->size(); ++i)
			{
				std::shared_ptr<T> item = m_pQueue->front();
				item.reset();
				m_pQueue->pop();
			}
			SAFE_DELETE(m_pQueue);
		}

	public:
		const std::vector<std::shared_ptr<T>>* GetSpawnedObjList()
		{
			return m_pSpawnedObjList;
		}
		std::shared_ptr<T> Spawn()
		{
			if (m_pQueue->size() == 0)
				this->ExpandPool();

			std::shared_ptr<T> item = m_pQueue->front();
			m_pQueue->pop();

			m_pSpawnedObjList->push_back(item);

			return item;
		}
		void Despawn(const std::shared_ptr<T>& _item)
		{
			int index = std::find(m_pSpawnedObjList->begin(), m_pSpawnedObjList->end(), _item) - m_pSpawnedObjList->begin();
			if (index == m_pSpawnedObjList->end())
				return;

			m_pQueue->push(_item);
			m_pSpawnedObjList->erase(m_pSpawnedObjList->begin() + index);
		}

	};
}