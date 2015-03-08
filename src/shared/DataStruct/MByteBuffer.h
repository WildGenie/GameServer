#ifndef __BYTEBUFFER_H
#define __BYTEBUFFER_H

#include <vector>
#include <list>
#include <string>
#include <map>

#include "MByteConverter.h"
#include "MSystemEndian.h"
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
	MByteBuffer(size_t len);
	~MByteBuffer();

	void setEndian(SysEndian endian);
	void clear();

	// 放入的值一定和系统大小端一样的
	template <typename T>
	void put(size_t pos, T value);

	MByteBuffer& writeUnsignedInt8(uint8 value);
	MByteBuffer& writeUnsignedInt16(uint16 value);
	MByteBuffer& writeUnsignedInt32(uint32 value);
	MByteBuffer& writeUnsignedInt64(uint64 value);
	// signed as in 2e complement
	MByteBuffer& writeInt8(int8 value);
	MByteBuffer& writeInt16(int16 value);
	MByteBuffer& writeInt32(int32 value);
	MByteBuffer& writeInt64(int64 value);
	// floating points
	MByteBuffer& writeFloat(float value);
	MByteBuffer& writeDouble(double value);
	// 写入 UTF-8 字符串，并且字符串中有 '\0' ，自己不用单独添加
	MByteBuffer& writeMultiByte(const std::string& value, size_t len);
	//MByteBuffer& writeMultiByte(const char* str, size_t len)
	MByteBuffer& writeBytes(const char* str, size_t startPos, size_t len);
	MByteBuffer& readBoolean(bool& value);
	MByteBuffer& readUnsignedInt8(uint8& value);
	MByteBuffer& readUnsignedInt16(uint16& value);
	MByteBuffer& readUnsignedInt32(uint32& value);
	MByteBuffer& readUnsignedInt64(uint64& value);
	// signed as in 2e complement
	MByteBuffer& readInt8(int8& value);
	MByteBuffer& readInt16(int16& value);
	MByteBuffer& readInt32(int32& value);
	MByteBuffer& readInt64(int64& value);
	MByteBuffer& readFloat(float& value);
	MByteBuffer& readDouble(double& value);
	MByteBuffer& readMultiByte(std::string& value, size_t len);
	MByteBuffer& readMultiByte(char* value, size_t len);

	template<class T>
	MByteBuffer& readUnused(MUnused<T> const&);

	uint8 operator[](size_t pos) const;
	size_t pos() const;
	size_t pos(size_t pos_);

	// 根据类型跳过
	template<typename T>
	void read_skip();

	// 根据大小跳过
	void read_skip(size_t skip);

	template<typename T>
	void write_skip();

	// 根据大小跳过
	void write_skip(size_t skip);

	template <typename T>
	T read();

	// 读取出来的一定是和系统大小端一样的
	template <typename T>
	T read(size_t pos) const;

	void read(uint8* dest, size_t len);
	const uint8* getStorage() const;
	size_t size() const;
	void setSize(size_t len);
	bool empty() const;
	size_t capacity();
	void readAddPos(int delta);
	void writeAddPos(int delta);
	void setCapacity(std::size_t newCapacity);

	/**
	*@brief 能否添加 num 长度的数据
	*/
	bool canAddData(uint32 num);
	// 在最后添加
	void append(const std::string& str);
	void append(const char* src, size_t cnt);

	template<class T>
	void append(const T* src, size_t cnt);

	void append(const uint8* src, size_t cnt);
	// 仅仅移动写指针，并不添加内容
	void append(size_t cnt);
	void append(const MByteBuffer& buffer);
	void put(size_t pos, const uint8* src, size_t cnt);
	char* rd_ptr();
	size_t avaliableBytes();
	void print_storage() const;
	void textlike() const;
	void hexlike() const;
	void writeFile(FILE* file);

	void compress(size_t startPos = 0, size_t len_ = 0);
	void uncompress(size_t startPos = 0, size_t len_ = 0);
	void encrypt(size_t startPos = 0, size_t len_ = 0);
	void decrype(size_t startPos = 0, size_t len_ = 0);

private:
	// 添加的一定是和系统大小端相同的
	// limited for internal use because can "append" any unexpected type (like pointer and etc) with hard detection problem
	template <typename T>
	void append(T value);

protected:
	size_t m_pos;		// 读取写入位置
	MStorageBuffer* m_pStorageBuffer;
	char m_encryptKey[8];
	char m_decryptKey[8];
};


#endif