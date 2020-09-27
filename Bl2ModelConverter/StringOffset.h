#pragma once
#include "Reader.h"

class StringOffset
{
public:
	int offset;
	int chunk;
	int size;
	std::string value;

	StringOffset(int offset, int chunk, FILE* file, const int size = 0);
	StringOffset();
private:
	FILE* f;
	void ReadData();
};

