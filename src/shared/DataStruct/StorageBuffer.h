#ifndef __STORE_BUFFER_H
#define __STORE_BUFFER_H

#include <cstddef>

class StorageBuffer
{
public:
	char* m_storage;
	std::size_t m_size;
	std::size_t m_iCapacity;

public:
	StorageBuffer(std::size_t len);
	~StorageBuffer();
	std::size_t size();
	std::size_t capacity();
	void setCapacity(std::size_t newCapacity);
	void setSize(std::size_t len);
	char* getStorage();
};

#endif			// __STORE_BUFFER_H