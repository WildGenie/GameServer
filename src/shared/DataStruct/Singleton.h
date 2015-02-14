#ifndef __SINGLETON_H
#define __SINGLETON_H

template<class T>
class Singleton
{
private:
	static T* m_sSingleton;

public:
	static T* getSingletonPtr()
	{
		if (m_sSingleton == NULL)
		{
			m_sSingleton = new T();
		}
		return m_sSingleton;
	}
};

#endif				// __SINGLETON_H