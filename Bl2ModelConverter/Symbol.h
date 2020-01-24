#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;
/// Trb consists of Symbols
///
/// Each Symbol has this Header
class Symbol
{
public:
	struct SymbolHeader
	{
		///Symbol Label
		string label;
		///Padding
		uint32_t zero;
		///Just a guess
		uint16_t relocationDataCount;
		/// Size of a Symbol
		uint16_t symbolSize;
		///Padding
		uint32_t zero2;
		///Relative Offset to a string in the Text Section
		uint32_t textOffset;
		///Some hash
		int32_t hash;
		///Padding
		uint32_t zero3;
		///Padding
		uint32_t zero4;
	};

	SymbolHeader symbolHeader;
	
};

