#pragma once
#include "umHalf.h"
#include <vector>
#include "FbxHelper.h"
#include "StringOffset.h"

using namespace System::Runtime::InteropServices;

class trb2
{
public:
	struct Props
	{
		std::vector<std::string> entityNames;
		std::vector<std::vector<std::string>> propNames;
		std::vector<std::vector<std::string>> types;
		std::vector<std::vector<std::string>> values;
	};
	trb2(std::string fileName, uint32_t type); //Constructor
	void readHeader();
	std::vector<std::string> readListBoxInfos();
	std::string readData(std::vector<int> indices, std::vector<std::string> fns);
	void writeScoringData(float scoringValue, int index);
	std::vector<std::wstring> ReadLocaleStrings();
	struct Props ReadProperties();
	~trb2();
protected:
	struct TagInfo
	{
		std::string tag;
		uint32_t dataOffset;
		uint32_t flag; //?
		uint32_t textOffset;
		std::string filename;
	};

	struct DataInfo
	{
		uint32_t unknown1;
		StringOffset textOffset;
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
		std::string scoringName;
		uint32_t scoringNameOffset;
		uint32_t unknown;
		float value;
	};

	struct EntitiesInfo
	{
		uint32_t entityNameOffset;
		uint16_t propertiesCount;
		uint16_t propertiesCount2; // IDK what that is
		uint32_t propertiesOffset; // Each Prop is 12 bytes
		uint32_t matrixOffset; //4x4 matrix
		uint32_t positionOffset; // 4 float vector (compare with Nicktoons Unite Entities .INI or .TRB) 
		uint32_t unknown;
		uint32_t flag;
		uint32_t valuesOffset; // 2 ints unknown purpose
	};

	enum PrimitiveType
	{
		ENUM, //OrientationType, VehicleType maybe enums? OnPointTrigger has FF FF FF FF?? INT
		INT,
		FLOAT,
		BOOL,
		TEXTOFFSET,
		VECTOR4,
		Unknown2,
		Unknown3,
		Unknown4,
		OFFSET
	};

	struct Property
	{
		uint32_t propertyNameOffset;
		PrimitiveType type;
	};

	

	std::vector<TagInfo> tagInfos;
	std::vector<SYMBInfo> symbInfos;
	std::vector<SubInfoData> subInfoDatas;
	std::vector<std::string> text;
	std::vector<uint32_t> subInfoStarts;
public:
	std::vector<ScoringInfo> si;
	  uint32_t scoringInfoOffset;
	  std::vector<DataInfo> dataInfos;
	  FILE* f;
	  uint32_t Type;
	  std::string filename;
};
