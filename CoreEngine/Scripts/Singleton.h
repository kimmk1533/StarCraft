#pragma once

namespace CoreEngine
{
	template <class T>
	class C_Singleton
	{
	private:
		static T* m_Instance;

	protected:
		C_Singleton();
		virtual ~C_Singleton();

		C_Singleton(const C_Singleton<T>& _other) = delete;
		void operator=(const C_Singleton<T>& _other) = delete;

	public:
		static T& GetI();
	};
}