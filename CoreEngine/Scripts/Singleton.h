#pragma once

template <class T>
class C_Singleton
{
protected:
	C_Singleton() = default;

	C_Singleton(const C_Singleton& other) = delete;

public:
	static T& GetI()
		;
	/*{
		static T Instance;
		return Instance;
	}*/
};