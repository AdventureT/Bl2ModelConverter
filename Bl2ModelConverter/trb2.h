#pragma once
#include "Reader.h"
#include "umHalf.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <tchar.h>
#include "FbxHelper.h"
#include "TrbHeader.h"
#include "PTEX.h"
#include "PMDL.h"

using namespace System::Runtime::InteropServices;
using namespace std;

class trb2 : Reader
{
public:
	trb2(char* fileName, uint32_t type); //Constructor
	void readHeader();
	vector<string> readListBoxInfos();
	string readData(vector<int> indices, vector<string> fns);
	void writeScoringData(float scoringValue, int index);
protected:

	struct TagInfo
	{
		string tag;
		uint32_t dataOffset;
		uint32_t flag; //?
		uint32_t textOffset;
	};

	struct DataInfo
	{
		uint32_t unknown1;
		uint32_t unknown2;
		uint32_t unknown3;
		uint32_t unknown4;
		uint32_t dataSize;
		uint32_t dataSize2;
		uint32_t dataOffset;
		uint32_t unknown5;
		uint32_t zero1;
		uint32_t zero2;
		uint32_t zero3;
		uint32_t zero4;
	};

	struct SubInfoData
	{
		uint32_t currentPMDLOffset;
		uint16_t meshIndex1;
		uint16_t meshIndex2;
		uint32_t unknown3;
		uint32_t unknown4;
		uint32_t unknown5;
		uint32_t unknown6;
		uint32_t vertexCount;
		uint32_t unknown8;
		uint32_t previousFaceCount;
		uint32_t faceCount;
		uint32_t unknown11;
		uint32_t unknown12;
		uint32_t vertexOffsetRelative;
		uint32_t normalUVOffset;
		uint32_t faceOffset;
		uint32_t sameSizeorOffset2;
		uint32_t sameSizeorOffset3;
		uint32_t sameSizeorOffset4; //This is pretty weird because it's the same size or offset 6 times in a row for some reason...
		uint32_t sameSizeorOffset5;
		uint32_t sameSizeorOffset6;
	};

	struct SYMBInfo
	{
		uint16_t unknown;
		uint16_t unknown2;
		uint16_t unknown3;
		uint16_t unknown4;
		uint32_t unknown5; //offset?
	};

	struct SubCollsionInfo
	{
		uint32_t vertOffset;
		uint32_t faceOffset;
		uint32_t faceEndOffset;
		uint32_t unknownOffset; //Offset to 3 floats with last one 0.75 might be thighness or smth?
		uint32_t vertCount;
		uint32_t faceCount;
		uint32_t unknown;
		uint32_t unknown2;
	};

	struct ScoringInfo
	{
		string scoringName;
		uint32_t scoringNameOffset;
		uint32_t unknown;
		float value;
	};

	vector<TagInfo> tagInfos;
	vector<DataInfo> dataInfos;
	vector<SYMBInfo> symbInfos;
	vector<SubInfoData> subInfoDatas;
	vector<string> text;
	vector<uint32_t> subInfoStarts;
public: 
	vector<ScoringInfo> si;
	  uint32_t scoringInfoOffset;
	  FILE* f;
	  uint32_t Type;
	  char* filename = new char[256];
};
