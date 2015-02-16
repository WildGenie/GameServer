#ifndef __STORE_BUFFER_H
#define __STORE_BUFFER_H

#include <cstddef>
#include "Platform/Define.h"

class StorageBuffer
{
public:
	char* m_storage;
	std::size_t m_size;
	std::size_t m_iCapacity;

public:
	StorageBuffer(std::size_t len);
	~StorageBuffer();
	void setCapacity(std::size_t newCapacity);
	void setSize(std::size_t len);
	bool canAddData(uint32 num);
};

#endif			// __STORE_BUFFER_H