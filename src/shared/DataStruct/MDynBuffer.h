#ifndef __MDYNBUFFER_H
#define __MDYNBUFFER_H

#include <cstddef>

class StorageBuffer;

class MDynBuffer
{
	friend class MNetClientBuffer;

protected:
	StorageBuffer* m_pStorageBuffer;

public:
	MDynBuffer(std::size_t len);
	~MDynBuffer();
	std::size_t size();
	std::size_t capacity();
	void setCapacity(std::size_t newCapacity);
	void setSize(std::size_t len);
	char* getStorage();
	void push(char* pItem, std::size_t len);
};

#endif			// __DYNBUFFER_H