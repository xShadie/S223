#ifndef __INC_ETERLIB_SINGLETON_H__
#define __INC_ETERLIB_SINGLETON_H__

#include <cassert>

template <typename T> class CSingleton 
{ 
	static T * ms_singleton;
	
public: 
	CSingleton()
	{ 
		assert(!ms_singleton);
		ms_singleton = static_cast<T*>(this);
	} 

	virtual ~CSingleton()
	{ 
		assert(ms_singleton);
		ms_singleton = 0; 
	}

	__forceinline static T & Instance()
	{
		assert(ms_singleton);
		return (*ms_singleton);
	}

	__forceinline static T * InstancePtr()
	{ 
		return (ms_singleton);
	}

	__forceinline static T & instance()
	{
		assert(ms_singleton);
		return (*ms_singleton);
	}
};

template <typename T>
T* CSingleton <T>::ms_singleton = nullptr;

template<typename T>
using singleton = CSingleton<T>;

#endif
