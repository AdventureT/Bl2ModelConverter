#pragma once
#include <cstdio>
#include <string>
#include <type_traits>

template <class T>
class DataOffset
{
	
public:
	int offset;
	int chunk;
	T value;
	
	DataOffset(int offset, int chunk, FILE* file)
	{
		this->offset = offset;
		this->chunk = chunk;
		this->f = file;
		ReadData();
	}
private:
	FILE* f;
	void ReadData()
	{
		fseek(f, chunk + offset, SEEK_SET);
		fread_s(&value, sizeof(value), sizeof(value), 1, f);
	}
	
};



