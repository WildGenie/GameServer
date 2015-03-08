#ifndef __COMPRESS_H
#define __COMPRESS_H

#include "Platform/Define.h"

// —πÀıΩ‚—πÀı
namespace NSCompress
{
	enum CompressMethod
	{
		DEFLAT,			// DEFLAT
		LZMA,			// LZMA
	};
}

class CompressUtil
{
public:
	static int mcompress(const char* srcBytes, uint32 srcLen, char** destBytes, uint32* destLen, NSCompress::CompressMethod method = NSCompress::DEFLAT);
	static int muncompress(const char* srcBytes, uint32 srcLen, char** destBytes, uint32* destLen, NSCompress::CompressMethod method = NSCompress::DEFLAT);
};


#endif