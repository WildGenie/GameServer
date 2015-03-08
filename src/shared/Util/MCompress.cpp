#include "MCompress.h"
#include "zlib/zlib.h"

#include <stdlib.h>

extern int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
extern int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

int CompressUtil::mcompress(const char* srcBytes, uint32 srcLen, char** destBytes, uint32* destLen, NSCompress::CompressMethod method)
{
	int32 ret = -1;
	if (method == NSCompress::DEFLAT)
	{
		int32 tmpSize = srcLen;
		while (ret != Z_OK)
		{
			if (*destBytes != nullptr)
			{
				free(*destBytes);
			}
			*destBytes = (char*)malloc(tmpSize * 2);
			tmpSize *= 2;
			ret = compress((Bytef *)destBytes, (uLongf *)(*destLen), (const Bytef *)srcBytes, srcLen);
		}
	}

	return ret;
}

int CompressUtil::muncompress(const char* srcBytes, uint32 srcLen, char** destBytes, uint32* destLen, NSCompress::CompressMethod method)
{
	int32 ret = -1;
	if (method == NSCompress::DEFLAT)
	{
		int32 ret = -1;
		int32 tmpSize = srcLen;
		while (ret != Z_OK)
		{
			if (*destBytes != nullptr)
			{
				free(*destBytes);
			}
			*destBytes = (char*)malloc(tmpSize * 2);
			tmpSize *= 2;
			ret = uncompress((Bytef *)destBytes, (uLongf *)(*destLen), (const Bytef *)srcBytes, srcLen);
		}
	}

	return ret;
}