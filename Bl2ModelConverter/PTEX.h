#pragma once
#include "Symbol.h"
///Prime Texture
class PTEX : public Symbol
{
public:
	struct TextureInfo
	{
		int32_t FFFFFFFF;
		uint32_t zero;
		uint8_t flags[12];
		///Just a Guess
		uint32_t count;
		uint32_t zero2;
		uint32_t zero3;
		uint32_t zero4;
		uint32_t zero5;
		uint32_t zero6;
		uint32_t unknown1;
		uint32_t zero7;
		uint32_t unknown2;
		uint32_t width;
		uint32_t height;
		uint8_t flags2[4];
		///Relative Offset to dds data in the DDS Section 
		uint32_t ddsOffset;
		///Size of dds data 
		uint32_t ddsSize;
		uint32_t zero8;
		uint32_t zero9;
		uint32_t zero10;
		///Just a Guess
		uint32_t unknown3;
		uint32_t zeros[13];
	};
	TextureInfo texInfo;
};

