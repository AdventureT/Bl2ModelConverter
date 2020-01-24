#pragma once
#include "Symbol.h"
/// Prime Model
class PMDL : public Symbol
{
public:
	struct Header
	{
		uint32_t unknown;
		uint32_t coordsCount;
		uint32_t coordsOffset; //2 XYZ float values
		uint32_t zero1;
		uint32_t zero2;
		uint32_t unknown2;
		uint32_t sumVertCount; // I don't think that's correct
		uint32_t faceStartOffsetRelative;
		uint32_t vertexStartOffset;
		uint32_t zero3;
		uint32_t sumFaceCount;
		uint32_t faceStartOffset;
		uint32_t zero4;
		uint32_t unknownCount;
		uint32_t unknownOffset; //Unknown
		uint32_t zero5;
	};

	struct Coords
	{
		float XYZW[4];
		float XYZW2[4];
	};

	struct Coords2
	{
		float XYZW11[4];
		float XYZW12[4];
		float XYZW21[4];
		float XYZW22[4];
	};

	struct UnknownTable
	{
		///TODO Find out what this is used for
	};

	struct MeshInfoTable
	{
		///This is the mesh count
		uint32_t meshInfoCount;
		///It is as ridiculous as it sounds
		uint32_t meshInfoOffsetsOffset;
		///Some coordinates
		uint32_t coords2Offset;
		uint32_t unknownTableCount;
		uint32_t unknownTableOffset;
	};

	struct MeshInfo
	{
		///Offset to the current PMDL for some reason
		uint32_t pmdlOffset;
		///I believe it's some sort of mesh index
		uint16_t meshIndex1;
		///I believe it's some sort of mesh index
		uint16_t meshIndex2;
		uint32_t unknown3;
		uint32_t unknown4;
		uint32_t unknown5;
		uint32_t unknown6;
		///Count of Vertices
		uint32_t vertexCount;
		uint32_t unknown8;
		///Previous Face Count
		uint32_t previousFaceCount;
		///Current Face Count
		uint32_t faceCount;
		uint8_t unknownIndex1;
		uint8_t unknownIndex2;
		///(Press F to Pay respect.) No clue what that is: Signed short = -1
		int16_t FFFF; 
		///(Press F to Pay respect.) No clue what that is: Signed int = -1
		int32_t FFFFFFFF; 
		uint32_t vertexOffsetRelative;
		uint32_t normalUVOffset;
		uint32_t faceOffset;
		///This is pretty weird because it's the same size or offset 6 times in a row for some reason...
		uint32_t sameSizeorOffset2;
		uint32_t sameSizeorOffset3;
		uint32_t sameSizeorOffset4; 
		uint32_t sameSizeorOffset5;
		uint32_t sameSizeorOffset6;
	};

	Header header;
	Coords coords;
	Coords2 coords2;
	UnknownTable unknownTable;
	std::vector<uint32_t> meshInfoOffsets;
	MeshInfoTable meshInfoTable;
	std::vector<MeshInfo> meshInfos;

};

