#pragma once

namespace CoreEngine
{
	template <class T>
	class C_Singleton
	{
	private:
		static T* m_Instance;

	protected:
		C_Singleton() {	}
		virtual ~C_Singleton() { }

		C_Singleton(const C_Singleton<T>& _other) = delete;
		void operator=(const C_Singleton<T>& _other) = delete;

	protected:
		bool CheckInstance() { return m_Instance == nullptr; }

	public:
		static T* GetI()
		{
			if (m_Instance == nullptr)
				m_Instance = new T();

			return m_Instance;
		};
		static void Destroy()
		{
			SAFE_DELETE(m_Instance);
		}
	};

	template <class T>
	T* C_Singleton<T>::m_Instance = nullptr;

}