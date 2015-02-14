#include "MyProject.h"
#include "BufferUtil.h"
#include <assert.h>

void* BufferUtil::memSwap(void *dest, const void *source, std::size_t count)
{
	assert((NULL != dest) && (NULL != source));

	char *tmp_source, *tmp_dest;
	char tmpChar;

	tmp_source = (char *)source;
	tmp_dest = (char *)dest;

	// ���û���ص�����
	while (count--)
	{
		tmpChar = *tmp_dest;
		*tmp_dest = *tmp_source;
		*tmp_source = tmpChar;
		++tmp_dest;
		++tmp_source;
	}

	return dest;
}
