#ifndef __BUFFERUTIL_H
#define __BUFFERUTIL_H

class BufferUtil
{
public:
	static void* memSwap(void *dest, const void *source, size_t count);
};

#endif		// __BUFFERUTIL_H