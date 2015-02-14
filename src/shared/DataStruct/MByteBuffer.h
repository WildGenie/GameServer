#ifndef __BYTEBUFFER_H
#define __BYTEBUFFER_H

#include <vector>
#include <list>
#include <string>
#include <map>

#include "ByteConverter.h"
#include "Error.h"
#include "SystemEndian.h"
#include "System.h"
#include "Platform/Define.h"


class ByteBufferException
{
    public:
        ByteBufferException(bool _add, size_t _pos, size_t _esize, size_t _size)
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
struct Unused
{
    Unused() {}
};

class MByteBuffer
{
protected:
	SysEndian m_sysEndian;

public:
  //  // constructor
  //  ByteBuffer(): _rpos(0), _wpos(0)
  //  {
		//m_sysEndian = eSys_LITTLE_ENDIAN;		// 默认是小端
		//_storage.reserve(INITCAPACITY);
  //  }

    // constructor
	MByteBuffer(size_t res) : _rpos(0), _wpos(0)
    {
		m_sysEndian = eSys_LITTLE_ENDIAN;		// 默认是小端
        _storage.reserve(res);
    }

    // copy constructor
	MByteBuffer(const MByteBuffer& buf) : _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage)
	{
		m_sysEndian = eSys_LITTLE_ENDIAN;		// 默认是小端
	}

    void clear()
    {
        _storage.clear();
        _rpos = _wpos = 0;
    }

	// 放入的值一定和系统大小端一样的
    template <typename T>
	void put(size_t pos, T value)
    {
		if (System::getSingletonPtr()->isEndianDiffFromSys(m_sysEndian))
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
        // append((uint8)0);
        return *this;
    }

	MByteBuffer& writeMultiByte(const char* str, size_t len)
    {
        //append((uint8 const*)str, str ? strlen(str) : 0);
        // append((uint8)0);
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

	MByteBuffer& readUnsigneduint8(uint8& value)
    {
        value = read<uint8>();
        return *this;
    }

	MByteBuffer& readUnsigneduint16(uint16& value)
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
        //value.clear();
        //while (rpos() < size())                         // prevent crash at wrong string format in packet
        //{
        //    char c = read<char>();
        //    if (c == 0)
        //        break;
        //    value += c;
        //}

		value.clear();
		if (len)		// 如果不为 0 ，就读取指定数量
		{
			size_t readNum = 0;	// 已经读取的数量
			while (rpos() < size() && readNum < len)                         // prevent crash at wrong string format in packet
			{
				char c = read<char>();
				value += c;
				++readNum;
			}
		}
		else				// 如果为 0 ，就一直读取，直到遇到第一个 '\0'
		{
			while (rpos() < size())                         // prevent crash at wrong string format in packet
			{
				char c = read<char>();
				if (c == 0)
					break;
				value += c;
			}
		}

        return *this;
    }

    template<class T>
	MByteBuffer& readUnused(Unused<T> const&)
    {
        read_skip<T>();
        return *this;
    }

    uint8 operator[](size_t pos) const
    {
        return read<uint8>(pos);
    }

    size_t rpos() const { return _rpos; }

    size_t rpos(size_t rpos_)
    {
        _rpos = rpos_;
        return _rpos;
    }

    size_t wpos() const { return _wpos; }

    size_t wpos(size_t wpos_)
    {
        _wpos = wpos_;
        return _wpos;
    }

	// 根据类型跳过
    template<typename T>
    void read_skip() { read_skip(sizeof(T)); }

	// 根据大小跳过
    void read_skip(size_t skip)
    {
        if (_rpos + skip > size())
            throw ByteBufferException(false, _rpos, skip, size());
        _rpos += skip;
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
        T r = read<T>(_rpos);
        _rpos += sizeof(T);
        return r;
    }

	// 读取出来的一定是和系统大小端一样的
    template <typename T>
	T read(size_t pos) const
    {
        if (pos + sizeof(T) > size())
            throw ByteBufferException(false, pos, sizeof(T), size());
        T val = *((T const*)&_storage[pos]);
		if (System::getSingletonPtr()->isEndianDiffFromSys(m_sysEndian))
		{
			EndianConvert(val);
		}
        return val;
    }

    void read(uint8* dest, size_t len)
    {
        if (_rpos  + len > size())
            throw ByteBufferException(false, _rpos, len, size());
        memcpy(dest, &_storage[_rpos], len);
        _rpos += len;
    }

    const uint8* contents() const { return &_storage[0]; }

    size_t size() const { return _storage.size(); }
    bool empty() const { return _storage.empty(); }

    void resize(size_t newsize)
    {
		if (newsize > size())
		{
			_storage.resize(newsize);
			_rpos = 0;
			_wpos = size();
		}
    }

    void reserve(size_t ressize)
    {
        if (ressize > size())
            _storage.reserve(ressize);
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
            return;

		assert(size() < 10000000);

        if (_storage.size() < _wpos + cnt)
            _storage.resize(_wpos + cnt);
        memcpy(&_storage[_wpos], src, cnt);
        _wpos += cnt;
    }

	// 仅仅移动写指针，并不添加内容
	void append(size_t cnt)
	{
		if (!cnt)
			return;

		assert(size() < 10000000);

		if (_storage.size() < _wpos + cnt)
			_storage.resize(_wpos + cnt);
		_wpos += cnt;
	}

	void append(const MByteBuffer& buffer)
    {
        if (buffer.wpos())
            append(buffer.contents(), buffer.wpos());
    }

    void put(size_t pos, const uint8* src, size_t cnt)
    {
        if (pos + cnt > size())
            throw ByteBufferException(true, pos, cnt, size());
        memcpy(&_storage[pos], src, cnt);
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
		if (System::getSingletonPtr()->isEndianDiffFromSys(m_sysEndian))
		{
			EndianConvert(value);
		}
        append((uint8*)&value, sizeof(value));
    }

protected:
	size_t _rpos;		// 读取位置
	size_t _wpos;		// 写入位置
    std::vector<uint8> _storage;		// 存储空间
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
    //*this >> temp;
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