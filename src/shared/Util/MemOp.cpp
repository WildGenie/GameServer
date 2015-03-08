#include "MemOp.h"
#include <stdlib.h>

void MemOp::freeMem(void* pMem)
{
	free(pMem);
}