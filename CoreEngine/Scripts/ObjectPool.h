#pragma once
#include <queue>

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
		void ExpandPool();

	public:
		C_ObjectPool(int _capacity = 10);
		virtual ~C_ObjectPool();

	public:
		HRESULT Create() override;
		void	Destroy() override;

	public:
		const std::vector<std::shared_ptr<T>>* GetSpawnedObjList();
		std::shared_ptr<T> Spawn();
		void Despawn(const std::shared_ptr<T>& _item);

	};
}