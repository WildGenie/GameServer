#include "MCompress.h"
#include "zlib/zlib.h"

extern int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
extern int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

int EncryptDecryptUtil::mcompress(const char* srcBytes, uint32 srcLen, char* destBytes, uint32* destLen, NSCompress::CompressMethod method)
{
	if (method == NSCompress::DEFLAT)
	{
		return compress((Bytef *)destBytes, (uLongf *)destLen, (const Bytef *)srcBytes, srcLen);
	}

	return -1;
}

int EncryptDecryptUtil::muncompress(const char* srcBytes, uint32 srcLen, char* destBytes, uint32* destLen, NSCompress::CompressMethod method)
{
	if (method == NSCompress::DEFLAT)
	{
		return uncompress((Bytef *)destBytes, (uLongf *)destLen, (const Bytef *)srcBytes, srcLen);
	}

	return -1;
}