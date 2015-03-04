#ifndef __BYTEBUFFER_H
#define __BYTEBUFFER_H

#include <vector>
#include <list>
#include <string>
#include <map>

#include "MByteConverter.h"
#include "MSystemEndian.h"
#include "System.h"
#include "Platform/Define.h"
#include "MDynBufResizePolicy.h"
#include "MStorageBuffer.h"

class MByteBufferException
{
public:
	MByteBufferException(bool _add, size_t _pos, size_t _esize, size_t _size)
		: add(_add), pos(_pos), esize(_esize), size(_size)
	{
		PrintPosError();
	}

	void PrintPosError() const;
private:
	bool add;
	size_t pos;
	size_t esize;
	size_t size;
};

template<class T>
struct MUnused
{
	MUnused() {}
};

class MByteBuffer
{
protected:
	SysEndian m_sysEndian;

public:
	// constructor
	MByteBuffer(size_t len) : m_pos(0)
	{
		m_pStorageBuffer = new MStorageBuffer(len);
		m_sysEndian = eSys_LITTLE_ENDIAN;		// 默认是小端
	}

	~MByteBuffer()
	{
		if (m_pStorageBuffer)
		{
			delete m_pStorageBuffer;
		}
	}

	void setEndian(SysEndian endian)
	{
		m_sysEndian = endian;
	}

	void clear()
	{
		m_pStorageBuffer->m_size = 0;
		m_pos = 0;
	}

	// 放入的值一定和系统大小端一样的
	template <typename T>
	void put(size_t pos, T value)
	{
		if (sSysEndian != m_sysEndian)
		{
			EndianConvert(value);
		}
		put(pos, (uint8*)&value, sizeof(value));
	}

	MByteBuffer& writeUnsignedInt8(uint8 value)
	{
		append<uint8>(value);
		return *this;
	}

	MByteBuffer& writeUnsignedInt16(uint16 value)
	{
		append<uint16>(value);
		return *this;
	}

	MByteBuffer& writeUnsignedInt32(uint32 value)
	{
		append<uint32>(value);
		return *this;
	}

	MByteBuffer& writeUnsignedInt64(uint64 value)
	{
		append<uint64>(value);
		return *this;
	}

	// signed as in 2e complement
	MByteBuffer& writeInt8(int8 value)
	{
		append<int8>(value);
		return *this;
	}

	MByteBuffer& writeInt16(int16 value)
	{
		append<int16>(value);
		return *this;
	}

	MByteBuffer& writeInt32(int32 value)
	{
		append<int32>(value);
		return *this;
	}

	MByteBuffer& writeInt64(int64 value)
	{
		append<int64>(value);
		return *this;
	}

	// floating points
	MByteBuffer& writeFloat(float value)
	{
		append<float>(value);
		return *this;
	}

	MByteBuffer& writeDouble(double value)
	{
		append<double>(value);
		return *this;
	}

	// 写入 UTF-8 字符串，并且字符串中有 '\0' ，自己不用单独添加
	MByteBuffer& writeMultiByte(const std::string& value, size_t len)
	{
		if (len > value.length())
		{
			append((uint8 const*)value.c_str(), value.length());
			append(len - value.length());
		}
		else
		{
			append((uint8 const*)value.c_str(), len);
		}
		return *this;
	}

	//MByteBuffer& writeMultiByte(const char* str, size_t len)
	MByteBuffer& writeBytes(const char* str, size_t startPos, size_t len)
	{
		if (str)
		{
			size_t charLen = strlen(str) / sizeof(char);
			if (len > charLen)
			{
				append(str, charLen);
				append(len - charLen);
			}
			else
			{
				append(str, len);
			}
		}
		else
		{
			append(len);
		}
		return *this;
	}

	MByteBuffer& readBoolean(bool& value)
	{
		value = read<char>() > 0 ? true : false;
		return *this;
	}

	MByteBuffer& readUnsignedInt8(uint8& value)
	{
		value = read<uint8>();
		return *this;
	}

	MByteBuffer& readUnsignedInt16(uint16& value)
	{
		value = read<uint16>();
		return *this;
	}

	MByteBuffer& readUnsignedInt32(uint32& value)
	{
		value = read<uint32>();
		return *this;
	}

	MByteBuffer& readUnsignedInt64(uint64& value)
	{
		value = read<uint64>();
		return *this;
	}

	// signed as in 2e complement
	MByteBuffer& readInt8(int8& value)
	{
		value = read<int8>();
		return *this;
	}

	MByteBuffer& readInt16(int16& value)
	{
		value = read<int16>();
		return *this;
	}

	MByteBuffer& readInt32(int32& value)
	{
		value = read<int32>();
		return *this;
	}

	MByteBuffer& readInt64(int64& value)
	{
		value = read<int64>();
		return *this;
	}

	MByteBuffer& readFloat(float& value)
	{
		value = read<float>();
		return *this;
	}

	MByteBuffer& readDouble(double& value)
	{
		value = read<double>();
		return *this;
	}

	MByteBuffer& readMultiByte(std::string& value, size_t len)
	{
		value.clear();
		if (len)		// 如果不为 0 ，就读取指定数量
		{
			size_t readNum = 0;	// 已经读取的数量
			while (pos() < size() && readNum < len) // prevent crash at wrong string format in packet
			{
				char c = read<char>();
				value += c;
				++readNum;
			}
		}
		else				// 如果为 0 ，就一直读取，直到遇到第一个 '\0'
		{
			while (pos() < size()) // prevent crash at wrong string format in packet
			{
				char c = read<char>();
				if (c == 0)
					break;
				value += c;
			}
		}

		return *this;
	}

	MByteBuffer& readMultiByte(char* value, size_t len)
	{
		size_t readNum = 0;	// 已经读取的数量
		char c = 0;
		if (len)		// 如果不为 0 ，就读取指定数量
		{
			while (pos() < size() && readNum < len)                         // prevent crash at wrong string format in packet
			{
				c = read<char>();
				value[readNum] = c;
				++readNum;
			}
		}
		else				// 如果为 0 ，就一直读取，直到遇到第一个 '\0'
		{
			while (pos() < size()) // prevent crash at wrong string format in packet
			{
				c = read<char>();
				if (c == 0)
				{
					break;
				}
				value[readNum] = c;
				++readNum;
			}
		}

		return *this;
	}

	template<class T>
	MByteBuffer& readUnused(MUnused<T> const&)
	{
		read_skip<T>();
		return *this;
	}

	uint8 operator[](size_t pos) const
	{
		return read<uint8>(pos);
	}

	size_t pos() const { return m_pos; }

	size_t pos(size_t pos_)
	{
		m_pos = pos_;
		return m_pos;
	}

	// 根据类型跳过
	template<typename T>
	void read_skip() { read_skip(sizeof(T)); }

	// 根据大小跳过
	void read_skip(size_t skip)
	{
		if (m_pos + skip > size())
			throw MByteBufferException(false, m_pos, skip, size());
		m_pos += skip;
	}

	template<typename T>
	void write_skip() { write_skip(sizeof(T)); }

	// 根据大小跳过
	void write_skip(size_t skip)
	{
		append(skip);
	}

	template <typename T>
	T read()
	{
		T r = read<T>(m_pos);
		readAddPos(sizeof(T));
		return r;
	}

	// 读取出来的一定是和系统大小端一样的
	template <typename T>
	T read(size_t pos) const
	{
		if (pos + sizeof(T) > size())
			throw MByteBufferException(false, pos, sizeof(T), size());
		T val = *((T const*)&m_pStorageBuffer->m_storage[pos]);
		if (sSysEndian != m_sysEndian)
		{
			MEndianConvert(val);
		}
		return val;
	}

	void read(uint8* dest, size_t len)
	{
		if (m_pos + len > size())
			throw MByteBufferException(false, m_pos, len, size());
		memcpy(dest, &m_pStorageBuffer->m_storage[m_pos], len);
		readAddPos(len);
	}

	const uint8* getStorage() const
	{
		return (uint8*)m_pStorageBuffer->m_storage;
	}

	size_t size() const { return m_pStorageBuffer->m_size; }

	void setSize(size_t len)
	{
		m_pStorageBuffer->setSize(len);
	}

	bool empty() const { return m_pStorageBuffer->m_size == 0; }

	size_t capacity()
	{
		return m_pStorageBuffer->m_iCapacity;
	}

	void readAddPos(int delta)
	{
		m_pos += delta;
	}

	void writeAddPos(int delta)
	{
		m_pos += delta;
		m_pStorageBuffer->m_size += delta;
	}

	void setCapacity(std::size_t newCapacity)
	{
		m_pStorageBuffer->setCapacity(newCapacity);
	}

	/**
	*@brief 能否添加 num 长度的数据
	*/
	bool canAddData(uint32 num)
	{
		return m_pStorageBuffer->canAddData(num);
	}

	// 在最后添加
	void append(const std::string& str)
	{
		append((uint8 const*)str.c_str(), str.size() + 1);
	}

	void append(const char* src, size_t cnt)
	{
		return append((const uint8*)src, cnt);
	}

	template<class T>
	void append(const T* src, size_t cnt)
	{
		return append((const uint8*)src, cnt * sizeof(T));
	}

	void append(const uint8* src, size_t cnt)
	{
		if (!cnt)
		{
			return;
		}

		if (!canAddData(cnt))
		{
			uint32 closeSize = MDynBufResizePolicy::getCloseSize(cnt + size(), capacity());
			setCapacity(closeSize);
		}
		memcpy(&m_pStorageBuffer->m_storage[m_pos], src, cnt);
		writeAddPos(cnt);
	}

	// 仅仅移动写指针，并不添加内容
	void append(size_t cnt)
	{
		if (!cnt)
		{
			return;
		}

		if (!canAddData(cnt))
		{
			uint32 closeSize = MDynBufResizePolicy::getCloseSize(cnt + size(), capacity());
			setCapacity(closeSize);
		}
		writeAddPos(cnt);
	}

	void append(const MByteBuffer& buffer)
	{
		if (buffer.pos())
			append(buffer.getStorage(), buffer.size());
	}

	void put(size_t pos, const uint8* src, size_t cnt)
	{
		if (!canAddData(cnt))
		{
			throw MByteBufferException(true, pos, cnt, size());
		}
		memcpy(&m_pStorageBuffer->m_storage[pos], src, cnt);
	}

	char* rd_ptr()
	{
		return m_pStorageBuffer->m_storage + m_pos;
	}

	size_t avaliableBytes()
	{
		return m_pStorageBuffer->m_size - m_pos;
	}

	void print_storage() const;
	void textlike() const;
	void hexlike() const;
	void writeFile(FILE* file);

private:
	// 添加的一定是和系统大小端相同的
	// limited for internal use because can "append" any unexpected type (like pointer and etc) with hard detection problem
	template <typename T>
	void append(T value)
	{
		if (sSysEndian != m_sysEndian)
		{
			MEndianConvert(value);
		}
		append((uint8*)&value, sizeof(value));
	}

protected:
	size_t m_pos;		// 读取写入位置
	MStorageBuffer* m_pStorageBuffer;
};

template <typename T>
inline MByteBuffer& operator<<(MByteBuffer& b, std::vector<T> const& v)
{
	b << (uint32)v.size();
	for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		b << *i;
	}
	return b;
}

template <typename T>
inline MByteBuffer& operator>>(MByteBuffer& b, std::vector<T>& v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while (vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename T>
inline MByteBuffer& operator<<(MByteBuffer& b, std::list<T> const& v)
{
	b << (uint32)v.size();
	for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i)
	{
		b << *i;
	}
	return b;
}

template <typename T>
inline MByteBuffer& operator>>(MByteBuffer& b, std::list<T>& v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while (vsize--)
	{
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename K, typename V>
inline MByteBuffer& operator<<(MByteBuffer& b, std::map<K, V>& m)
{
	b << (uint32)m.size();
	for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i)
	{
		b << i->first << i->second;
	}
	return b;
}

template <typename K, typename V>
inline MByteBuffer& operator>>(MByteBuffer& b, std::map<K, V>& m)
{
	uint32 msize;
	b >> msize;
	m.clear();
	while (msize--)
	{
		K k;
		V v;
		b >> k >> v;
		m.insert(make_pair(k, v));
	}
	return b;
}

template<>
inline void MByteBuffer::read_skip<char*>()
{
	std::string temp;
	this->readMultiByte(temp, 0);
}

template<>
inline void MByteBuffer::read_skip<char const*>()
{
	read_skip<char*>();
}

template<>
inline void MByteBuffer::read_skip<std::string>()
{
	read_skip<char*>();
}


#endif