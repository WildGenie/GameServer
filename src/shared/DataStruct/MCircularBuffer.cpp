#include "MCircularBuffer.h"
#include "DynBufResizePolicy.h"
#include "BufferUtil.h"
#include <string.h>
#include "Platform/Define.h"

/**
 * @brief 构造函数
 */
MCircularBuffer::MCircularBuffer(std::size_t len)
	: m_head(0), m_tail(0), m_size(0), m_iCapacity(len)
{
	m_storage = new char[len];
}

MCircularBuffer::~MCircularBuffer()
{
	delete[] m_storage;
}

std::size_t MCircularBuffer::size()
{
	return m_size;
}

bool MCircularBuffer::full()
{
	return (m_iCapacity == m_size);
}

void MCircularBuffer::clear()
{
	m_head = 0;
	m_tail = 0;
	m_size = 0;
}

char* MCircularBuffer::getStorage()
{
	return m_storage;
}

bool MCircularBuffer::empty()
{
	return (m_size == 0);
}

void MCircularBuffer::linearize()
{
	if (isLinearized())
	{
		// 函数memcpy()   从source  指向的区域向dest指向的区域复制count个字符，如果两数组重叠，不定义该函数的行为。而memmove(), 如果两函数重叠，赋值仍正确进行。memcpy函数假设要复制的内存区域不存在重叠，如果你能确保你进行复制操作的的内存区域没有任何重叠，可以直接用memcpy；如果你不能保证是否有重叠，为了确保复制的正确性，你必须用memmove。memcpy的效率会比memmove高一些，如果还不明白的话可以看一些两者的实现： 函数memcpy()   从source  指向的区域向dest指向的区域复制count个字符，如果两数组重叠，不定义该函数的行为。而memmove(), 如果两函数重叠，赋值仍正确进行。	memcpy函数假设要复制的内存区域不存在重叠，如果你能确保你进行复制操作的的内存区域没有任何重叠，可以直接用memcpy；如果你不能保证是否有重叠，为了确保复制的正确性，你必须用memmove。memcpy的效率会比memmove高一些
		std::memmove(m_storage, m_storage + m_head, m_size);
	}
	else
	{
		if (m_iCapacity - m_head == m_tail)
		{
			BufferUtil::memSwap(m_storage, m_storage + m_head, m_tail);
		}
		else if (m_iCapacity - m_head > m_tail)
		{
			BufferUtil::memSwap(m_storage, m_storage + m_head, m_size - m_head);
			std::memmove(m_storage + (m_size - m_tail), m_storage + m_head, m_tail);
		}
		else
		{
			BufferUtil::memSwap(m_storage, m_storage + (m_size - m_tail), m_tail);
			std::memmove(m_storage, m_storage + (m_tail - m_head), m_head);
		}
	}

	m_head = 0;
	m_tail = m_size;
}

bool MCircularBuffer::isLinearized()
{
	return m_head <= m_tail;
}

size_t MCircularBuffer::capacity()
{
	return m_iCapacity;
}

void MCircularBuffer::setCapacity(std::size_t newCapacity)
{
	if (newCapacity == capacity())
	{
		return;
	}
	if (newCapacity < size())       // 不能分配比当前已经占有的空间还小的空间
	{
		return;
	}
	char* tmpbuff = new char[newCapacity];   // 分配新的空间
	if (isLinearized()) // 如果是在一段内存空间
	{
		std::memcpy(tmpbuff, m_storage + m_head, m_size);
	}
	else    // 如果在两端内存空间
	{
		std::memcpy(tmpbuff, m_storage + m_head, m_iCapacity - m_head);
		std::memcpy(tmpbuff + m_iCapacity - m_head, m_storage, m_tail);
	}

	m_head = 0;
	m_tail = m_size;
	m_iCapacity = newCapacity;

	delete[] m_storage;
	m_storage = tmpbuff;
}

/**
*@brief 能否添加 num 长度的数据
*/
bool MCircularBuffer::canAddData(uint32 num)
{
	if (m_iCapacity - m_size > num)
	{
		return true;
	}

	return false;
}

/**
* @brief 在缓冲区尾部添加
*/
void MCircularBuffer::pushBack(char* pItem, std::size_t startPos, std::size_t len)
{
	if (!canAddData(len)) // 存储空间必须要比实际数据至少多 1
	{
		uint32 closeSize = DynBufResizePolicy::getCloseSize(len + size(), capacity());
		setCapacity(closeSize);
	}

	if (isLinearized())
	{
		if (len <= (m_iCapacity - m_tail))
		{
			std::memcpy(m_storage + m_tail, pItem + startPos, len);
		}
		else
		{
			std::memcpy(m_storage + m_tail, pItem + startPos, m_iCapacity - m_tail);
			std::memcpy(m_storage + 0, pItem + m_iCapacity - m_tail, len - (m_iCapacity - m_tail));
		}
	}
	else
	{
		std::memcpy(m_storage + m_tail, pItem + startPos, len);
	}

	m_tail += len;
	m_tail %= m_iCapacity;

	m_size += len;
}

/**
* @brief 在缓冲区头部添加
*/
void MCircularBuffer::pushFront(char* pItem, std::size_t startPos, std::size_t len)
{
	if (!canAddData(len)) // 存储空间必须要比实际数据至少多 1
	{
		uint32 closeSize = DynBufResizePolicy::getCloseSize(len + size(), capacity());
		setCapacity(closeSize);
	}

	if (isLinearized())
	{
		if (len <= m_head)
		{
			std::memcpy(m_storage + m_head - len, pItem + startPos, len);
		}
		else
		{
			std::memcpy(m_storage + 0, pItem + startPos + len - m_head, m_head);
			std::memcpy(m_storage + m_iCapacity - (len - m_head), pItem + 0, len - m_head);
		}
	}
	else
	{
		std::memcpy(m_storage + m_head - len, pItem + startPos + 0, len);
	}

	if (len <= m_head)
	{
		m_head -= (uint32)len;
	}
	else
	{
		m_head = m_iCapacity - ((uint32)len - m_head);
	}
	m_size += (uint32)len;
}

/**
* @brief 获取缓冲区尾部，并且删除
*/
bool MCircularBuffer::popBack(char* pItem, std::size_t startPos, std::size_t len)
{
	if (back(pItem, startPos, len))
	{
		popBackLenNoData(len);
		return true;
	}

	return false;
}

/**
* @brief 获取缓冲区尾部，但是不删除
*/
bool MCircularBuffer::back(char* pItem, std::size_t startPos, std::size_t len)
{
	if (len <= size())
	{
		return false;
	}

	if (isLinearized())
	{
		std::memcpy(pItem + startPos, m_storage + m_tail - len, len);
	}
	else
	{
		if (len <= m_tail)
		{
			std::memcpy(pItem + startPos, m_storage, len);
		}
		else
		{
			std::memcpy(pItem + startPos, m_storage + m_iCapacity - (len - m_tail), len - m_tail);
			std::memcpy(pItem + startPos + len - m_tail, m_storage + 0, m_tail);
		}
	}

	return true;
}

void MCircularBuffer::popBackLenNoData(std::size_t len)
{
	m_tail -= len;
	m_tail += m_iCapacity;
	m_tail %= m_iCapacity;
	m_size -= len;
}

/**
* @brief 获取缓冲区头部，但是删除
*/
bool MCircularBuffer::popFront(char* pItem, std::size_t startPos, std::size_t len)
{
	if (front(pItem, startPos, len))
	{
		popFrontLenNoData(len);
		return true;
	}

	return false;
}

/**
* @brief 获取缓冲区头部，但是不删除
*/
bool MCircularBuffer::front(char* pItem, std::size_t startPos, std::size_t len)
{
	if (len <= size())
	{
		return false;
	}

	if (isLinearized())
	{
		std::memcpy(pItem + startPos, m_storage + m_head, len);
	}
	else
	{
		if (len <= (m_iCapacity - m_head))
		{
			std::memcpy(pItem + startPos, m_storage + m_head, len);
		}
		else
		{
			std::memcpy(pItem + startPos, m_storage + m_head, m_iCapacity - m_head);
			std::memcpy(pItem + startPos + m_iCapacity - m_head, m_storage + 0, len - (m_iCapacity - m_head));
		}
	}

	return true;
}

void MCircularBuffer::popFrontLenNoData(std::size_t len)
{
	m_head += len;
	m_head %= m_iCapacity;
	m_size -= len;
}