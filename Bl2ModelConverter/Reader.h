#pragma once
#include <stdio.h>
#include "umHalf.h"
#include <string>
#include <vector>
	class Reader
	{
	public:
		char ReadByte(FILE* f);

		unsigned char ReadUByte(FILE* f);

		char* ReadBytes(FILE* f, int amount);

		std::vector<unsigned char> ReadUBytes(FILE* f, int amount);

		short ReadShort(FILE* f);

		unsigned short ReadUShort(FILE* f);

		half ReadHalfFloat(FILE* f);

		float ReadFloat(FILE* f);

		long ReadLong(FILE* f);

		unsigned long ReadULong(FILE* f);

		long long ReadLongLong(FILE* f);

		unsigned long long ReadULongLong(FILE* f);

		std::string ReadString(FILE* f);
		std::string ReadString(FILE* f, int chars);
	};


