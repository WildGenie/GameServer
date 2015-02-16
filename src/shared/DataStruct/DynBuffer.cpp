#include "DynBuffer.h"
#include "DynBufResizePolicy.h"
#include <string.h>
#include "StorageBuffer.h"

DynBuffer::DynBuffer(std::size_t len)
{
	m_pStorageBuffer = new StorageBuffer(len);
}

DynBuffer::~DynBuffer()
{
	delete[] m_pStorageBuffer;
}

std::size_t DynBuffer::size()
{
	return m_pStorageBuffer->m_size;
}

size_t DynBuffer::capacity()
{
	return m_pStorageBuffer->m_iCapacity;
}

void DynBuffer::setSize(std::size_t len)
{
	m_pStorageBuffer->setSize(len);
}

void DynBuffer::setCapacity(std::size_t newCapacity)
{
	m_pStorageBuffer->setCapacity(newCapacity);
}

char* DynBuffer::getStorage()
{
	return m_pStorageBuffer->m_storage;
}

void DynBuffer::push(char* pItem, std::size_t len)
{
	if (len > m_pStorageBuffer->m_iCapacity)
	{
		uint32 closeSize = DynBufResizePolicy::getCloseSize(len, capacity());
		setCapacity(closeSize);
	}

	memcpy(m_pStorageBuffer->m_storage, pItem, len);
	m_pStorageBuffer->m_size = len;
}