#pragma once
#define SingletonHeader(Class) \
private: \
	static Class instance; \
	Class() {} \
public: \
	Class(const Class& c) = delete; \
	void operator=(const Class& c) = delete; \
	static Class& GetInstance() \
	{ \
		return instance; \
	} \

#define SingletonSource(Class) \
Class::Class() \
{ \
 \
} \
static Class& Class:GetInstance() \
{ \
	return instance; \
} \