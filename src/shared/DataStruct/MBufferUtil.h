#ifndef __MBUFFERUTIL_H
#define __MBUFFERUTIL_H

class MBufferUtil
{
public:
	static void* memSwap(void *dest, const void *source, size_t count);
};

#endif		// __BUFFERUTIL_H