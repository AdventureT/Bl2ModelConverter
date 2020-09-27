#include "StringOffset.h"

StringOffset::StringOffset(int offset, int chunk, FILE* file, const int size)
{
	this->offset = offset;
	this->chunk = chunk;
	this->f = file;
	this->size = size;
	ReadData();
}

StringOffset::StringOffset()
{

}

void StringOffset::ReadData()
{
	auto const ret = ftell(f);
	fseek(f, chunk + offset, SEEK_SET);
	if (size == 0)
	{
		value = ReadString(f);
	}
	else
	{
		value = ReadString(f, size);
	}
	fseek(f, ret, SEEK_SET);
}
