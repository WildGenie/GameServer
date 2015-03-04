#ifndef __MDYNBUFRESIZEPOLICY_H
#define __MDYNBUFRESIZEPOLICY_H

#include "Platform/Define.h"

class MDynBufResizePolicy
{
public:
	static uint32 getCloseSize(uint32 needSize, uint32 capacity);
};


#endif			// __DYNBUFRESIZEPOLICY_H	