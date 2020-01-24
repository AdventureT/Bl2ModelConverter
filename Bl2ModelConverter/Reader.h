#pragma once
#include <stdio.h>
#include "umHalf.h"
#include <string>
#include <vector>
using namespace std;
	class Reader
	{
	public:
		uint8_t ReadByte(FILE* f);

		vector<uint8_t> Reader::ReadBytes(FILE* f, int amount);

		short ReadShort(FILE* f);

		unsigned short ReadUShort(FILE* f);

		uint32_t ReadUInt(FILE* f);

		int32_t ReadInt(FILE* f);

		half ReadHalfFloat(FILE* f);

		float ReadFloat(FILE* f);

		long ReadLong(FILE* f);

		unsigned long ReadULong(FILE* f);

		long long ReadLongLong(FILE* f);

		unsigned long long ReadULongLong(FILE* f);

		std::string ReadString(FILE* f);
		std::string ReadString(FILE* f, int chars);
	};


