#include "MByteBuffer.h"

#include <sstream>

void MByteBufferException::PrintPosError() const
{
	
}

void MByteBuffer::print_storage() const
{
    std::ostringstream ss;
    ss <<  "STORAGE_SIZE: " << size() << "\n";
    ss << "         ";

    for (size_t i = 0; i < size(); ++i)
        ss << uint32(read<uint8>(i)) << " - ";
}

void MByteBuffer::textlike() const
{
    std::ostringstream ss;
    ss <<  "STORAGE_SIZE: " << size() << "\n";

    ss << "         ";

    for (size_t i = 0; i < size(); ++i)
        ss << read<uint8>(i);
}

void MByteBuffer::hexlike() const
{
    std::ostringstream ss;
    ss <<  "STORAGE_SIZE: " << size() << "\n";

    ss << "         ";

    size_t j = 1, k = 1;

    for (size_t i = 0; i < size(); ++i)
    {
        if ((i == (j * 8)) && ((i != (k * 16))))
        {
            ss << "| ";
            ++j;
        }
        else if (i == (k * 16))
        {
            ss << "\n";

            ss << "         ";

            ++k;
            ++j;
        }

        char buf[4];
        _snprintf(buf, 4, "%02X", read<uint8>(i));
        ss << buf << " ";
    }
}

void MByteBuffer::writeFile(FILE* file)
{
	fwrite((void*)&_storage[0], sizeof(char), this->size(), file);
}