#ifndef __DYNBUFRESIZEPOLICY_H
#define __DYNBUFRESIZEPOLICY_H

#include "Platform/Define.h"

class DynBufResizePolicy
{
public:
	static uint32 getCloseSize(uint32 needSize, uint32 capacity);
};


#endif			// __DYNBUFRESIZEPOLICY_H	