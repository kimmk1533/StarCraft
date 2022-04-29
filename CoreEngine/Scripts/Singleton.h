#pragma once

template <class T>
class Singleton
{
private:
	Singleton() { }
	~Singleton() { }

public:
	T& GetI()
	{
		static T Instance;
		return Instance;
	}
};