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

		C_Singleton(const C_Singleton& _other) = delete;

	public:
		static T& GetI();
	};

	/*template <class T>
	__interface ISingleton
	{
	protected:
		T();
		T(const T& _other) = delete;

	public:
		static virtual T& GetInstance() = 0;
	};*/
}