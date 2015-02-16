#ifndef __DYNBUFFER_H
#define __DYNBUFFER_H

#include <cstddef>

class StorageBuffer;

class DynBuffer
{
	friend class MNetClientBuffer;

protected:
	StorageBuffer* m_pStorageBuffer;

public:
	DynBuffer(std::size_t len);
	~DynBuffer();
	std::size_t size();
	std::size_t capacity();
	void setCapacity(std::size_t newCapacity);
	void setSize(std::size_t len);
	char* getStorage();
	void push(char* pItem, std::size_t len);
};

#endif			// __DYNBUFFER_H