#include "Reader.h"
char Reader::ReadByte(FILE* f)
{
	unsigned char read;
	fread_s(&read, 1, 1, 1, f);
	return read;
}

unsigned char Reader::ReadUByte(FILE* f)
{
	unsigned char read;
	fread_s(&read, 1, 1, 1, f);
	return read;
}

char* Reader::ReadBytes(FILE* f, int amount)
{
	char* read = new char[amount];
	fread_s(&read, amount, amount, 1, f);
	return read;
}

std::vector<unsigned char> Reader::ReadUBytes(FILE* f, int amount)
{
	std::vector<unsigned char> read;
	for (int i = 0; i < amount; i++)
	{
		unsigned char c;
		fread_s(&c, 1, 1, 1, f);
		read.push_back(c);
	}
	return read;
}


short Reader::ReadShort(FILE* f)
{
	short read;
	fread_s(&read, 2, 2, 1, f);
	return read;
}

unsigned short Reader::ReadUShort(FILE* f)
{
	unsigned short read;
	fread_s(&read, 2, 2, 1, f);
	return read;
}

half Reader::ReadHalfFloat(FILE* f)
{
	half read;
	fread_s(&read, 2, 2, 1, f);
	return read;
}

float Reader::ReadFloat(FILE* f)
{
	float read;
	fread_s(&read, 4, 4, 1, f);
	return read;
}

long Reader::ReadLong(FILE* f)
{
	long read;
	fread_s(&read, 4, 4, 1, f);
	return read;
}

unsigned long Reader::ReadULong(FILE* f)
{
	unsigned long read;
	fread_s(&read, 4, 4, 1, f);
	return read;
}

long long Reader::ReadLongLong(FILE* f)
{
	long long read;
	fread_s(&read, 8, 8, 1, f);
	return read;
}

unsigned long long Reader::ReadULongLong(FILE* f)
{
	unsigned long long read;
	fread_s(&read, 8, 8, 1, f);
	return read;
}

std::string Reader::ReadString(FILE* f)
{
	std::string read;
	char c;
	do
	{
		fread_s(&c, 1, 1, 1, f);
		read.push_back(c);
	} while (c != '\0');
	return read;
}
std::string Reader::ReadString(FILE* f, int chars)
{
	std::string read;
	char c;
	for (int i = 0; i < chars; i++)
	{
		fread_s(&c, 1, 1, 1, f);
		read.push_back(c);
	}
	return read;
}