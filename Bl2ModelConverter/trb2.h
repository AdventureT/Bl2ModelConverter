#pragma once
#include "Reader.h"
#include "umHalf.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fbxsdk.h>
#include <tchar.h>

namespace Trb2
{
	using namespace System::Runtime::InteropServices;
	using namespace std;
	class trb2 : Reader
	{
	public:
		trb2(char* fileName, int type); //Constructor
		void readHeader();
		vector<string> readListBoxInfos();
		void readData(vector<int> indices, std::vector<std::string> fns);
	protected:
		struct TagInfo
		{
			std::string tag;
			long dataOffset;
			long one;
			long textOffset;
		};

		struct Header
		{
			std::string magic;
			long magic2;
			short flag1;
			short flag2;
			long dataInfoCount;
			long dataInfoSize;
			long tagCount;
			long tagSize; //Unknown
			long relocationDataOffset;
			long relocationDataSize;
		};

		struct DataInfo
		{
			long unknown1;
			long unknown2;
			long unknown3;
			long unknown4;
			long dataSize;
			long dataSize2;
			long dataOffset;
			long unknown5;
			long zero1;
			long zero2;
			long zero3;
			long zero4;
		};

		struct SubInfoData
		{
			long unknown1;
			long unknown2;
			long unknown3;
			long unknown4;
			long unknown5;
			long unknown6;
			long vertexCount;
			long unknown8;
			long previousFaceCount;
			long faceCount;
			long unknown11;
			long unknown12;
			long vertexOffsetRelative;
			long normalUVOffset;
			long faceOffset;
			long sameSizeorOffset2;
			long sameSizeorOffset3;
			long sameSizeorOffset4; //This is pretty weird because it's the same size or offset 6 times in a row for some reason...
			long sameSizeorOffset5;
			long sameSizeorOffset6;
		};

		struct SYMBInfo
		{
			short unknown;
			short unknown2;
			short unknown3;
			short unknown4;
			long unknown5; //offset?
		};

		struct SubCollsionInfo
		{
			long vertOffset;
			long faceOffset;
			long faceEndOffset;
			long unknownOffset; //Offset to 3 floats with last one 0.75 might be thighness or smth?
			long vertCount;
			long faceCount;
			long unknown;
			long unknown2;
		};

		std::vector<TagInfo> tagInfos;
		std::vector<DataInfo> dataInfos;
		std::vector<SYMBInfo> symbInfos;
		std::vector<SubInfoData> subInfoDatas;
		std::vector<std::string> text;
		std::vector<long> subInfoStarts;
		FILE* f;
		int Type;
		char* filename = new char[256];
	};


}
