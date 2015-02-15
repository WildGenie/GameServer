#ifndef __BYTECONVERTER_H
#define __BYTECONVERTER_H

/** 
 *@brief ByteConverter reverse your byte order.  This is use for cross platform where they have different endians.
 */
#include <algorithm>
#include "Platform/Define.h"

namespace MByteConverter
{
	template<size_t T>
	inline void convert(char* val)
	{
		std::swap(*val, *(val + T - 1));
		convert < T - 2 >(val + 1);
	}

	template<> inline void convert<0>(char*) {}
	template<> inline void convert<1>(char*) {}             // ignore central byte

	template<typename T>
	inline void apply(T* val)
	{
		convert<sizeof(T)>((char*)(val));
	}
}

template<typename T> inline void MEndianConvert(T& val) { MByteConverter::apply<T>(&val); }
template<typename T> void MEndianConvert(T*);         // will generate link error

inline void MEndianConvert(uint8&) { }
inline void MEndianConvert(int8&)  { }

#endif
