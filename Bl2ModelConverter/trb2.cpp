#include "trb2.h"


Trb2::trb2::trb2(char* fileName, int type)
{
	filename = fileName;
	Type = type;
	//read();
}

void Trb2::trb2::readHeader()
{
	if (fopen_s(&f, filename, "rb") != 0) // Security check
	{
		throw gcnew System::Exception(gcnew System::String("Can't open the trb file"));
	}
	Header header;
	header.magic = ReadString(f);
	header.magic2 = ReadLong(f);
	header.flag1 = ReadShort(f);
	header.flag2 = ReadShort(f);
	header.dataInfoCount = ReadLong(f);
	header.dataInfoSize = ReadLong(f);
	header.tagCount = ReadLong(f);
	header.tagSize = ReadLong(f); //Unknown
	header.unknownOffset = ReadLong(f); //SYMB Stride: 12 Count: header.unknownSize / 12?
	header.unknownSize = ReadLong(f);
	fseek(f, 92, SEEK_CUR);
	for (int i = 0; i < header.dataInfoCount; i++)
	{
		DataInfo dataInfo = { ReadLong(f), ReadLong(f), ReadLong(f) , ReadLong(f) , ReadLong(f),
		ReadLong(f), ReadLong(f), ReadLong(f) , ReadLong(f) , ReadLong(f), ReadLong(f), ReadLong(f) };
		dataInfos.push_back(dataInfo);
	}
	for (int i = 0; i < header.tagCount; i++)
	{
		TagInfo tagInfo{ ReadString(f,4), ReadLong(f), ReadLong(f), ReadLong(f) };
		tagInfos.push_back(tagInfo);
	}
	fseek(f, dataInfos[0].dataOffset, SEEK_SET);
	while (ftell(f) < dataInfos[0].dataOffset + dataInfos[0].dataSize)
	{
		text.push_back(ReadString(f));
	}
	fseek(f, header.unknownOffset, SEEK_SET);
	for (int i = 0; i < header.unknownSize / 12; i++)
	{
		SYMBInfo symbInfo = { ReadShort(f), ReadShort(f), ReadShort(f),ReadShort(f), ReadLong(f) };
		symbInfos.push_back(symbInfo);
	}
}

std::vector<std::string> Trb2::trb2::readListBoxInfos()
{
	vector<string> infos;
	for (int i = 0; i < tagInfos.size(); i++)
	{
		infos.push_back(tagInfos[i].tag); //tag
		fseek(f, tagInfos[i].textOffset + dataInfos[0].dataOffset, SEEK_SET);
		infos.push_back(ReadString(f)); //filename
		infos.push_back((char*)(void*)Marshal::StringToHGlobalAnsi(i.ToString()));
	}
	return infos;
}

void Trb2::trb2::readData(vector<int> indices, std::vector<std::string> fns)
{
	for (int i = 0; i < indices.size(); i++)
	{
		fseek(f, tagInfos[indices[i]].dataOffset + dataInfos[1].dataOffset, SEEK_SET);
		if (tagInfos[indices[i]].tag == "PMDL")
		{
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> uvs;
			std::vector<unsigned short> faces;
			std::string tag = ReadString(f, 4);
			long zero = ReadLong(f);
			short elefen = ReadShort(f);
			short ninetysix = ReadShort(f);
			zero = ReadLong(f);
			long modelTextOffset = ReadLong(f);
			fseek(f, 36, SEEK_CUR);
			long sumVertCount = ReadLong(f);
			long faceStartOffsetRelative = ReadLong(f);
			long vertexStartOffset = ReadLong(f) + dataInfos[2].dataOffset;
			fseek(f, 4, SEEK_CUR);
			long sumFaceCount = ReadLong(f);
			long faceStartOffset = ReadLong(f);
			fseek(f, 48, SEEK_CUR);
			long SubInfoCount = ReadLong(f);
			long offsetToSubInfosStart = ReadLong(f);
			long endOfSubInfos = ReadLong(f);
			/*
			fseek(f, modelTextOffset + dataInfos[0].dataOffset, SEEK_SET);
			std::string modelName = ReadString(f);
			std::string geometry = ReadString(f);
			std::string name = ReadString(f);
			*/
			fseek(f, offsetToSubInfosStart + dataInfos[1].dataOffset, SEEK_SET);
			for (int b = 0; b < SubInfoCount; b++)
			{
				subInfoStarts.push_back(ReadLong(f));
			}
			long normalUVStart = 0;
			long normalUVEnd = 0;
			std::vector<long> normalUVSize;
			for (int a = 0; a < SubInfoCount; a++)
			{
				fseek(f, subInfoStarts[a] + dataInfos[1].dataOffset, SEEK_SET);
				SubInfoData subInfoData = { ReadLong(f), ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),
					ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),
					ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f),ReadLong(f) };
				subInfoDatas.push_back(subInfoData);
			}
			if (SubInfoCount > 1)
			{
				int remember = 0;
				for (int a = 0; a + 1 < SubInfoCount; a++)
				{
					normalUVStart = subInfoDatas[a].normalUVOffset;
					normalUVEnd = subInfoDatas[a + 1].normalUVOffset;
					normalUVSize.push_back(normalUVEnd - normalUVStart);
					remember = a;
				}
				normalUVStart = subInfoDatas[remember + 1].normalUVOffset;
				normalUVEnd = subInfoDatas[remember + 1].faceOffset;
				normalUVSize.push_back(normalUVEnd - normalUVStart);
			}
			else if (SubInfoCount == 1)
			{
				normalUVStart = subInfoDatas[0].normalUVOffset;
				normalUVEnd = subInfoDatas[0].faceOffset;
				normalUVSize.push_back(normalUVEnd - normalUVStart);
			}
			if (SubInfoCount >= 1)
			{
				for (int x = 0; x < SubInfoCount; x++)
				{
					int stride = normalUVSize[x] / subInfoDatas[x].vertexCount;
					fseek(f, vertexStartOffset + subInfoDatas[x].vertexOffsetRelative, SEEK_SET);
					for (int j = 0; j < subInfoDatas[x].vertexCount; j++)
					{
						float vertexX = ReadHalfFloat(f);
						float vertexY = ReadHalfFloat(f);
						float vertexZ = ReadHalfFloat(f);
						float vertexW = ReadHalfFloat(f);

						vertices.push_back(vertexX);
						vertices.push_back(vertexY);
						vertices.push_back(vertexZ);
						vertices.push_back(vertexW);
					}
					fseek(f, vertexStartOffset + subInfoDatas[x].normalUVOffset, SEEK_SET);
					for (int z = 0; z < subInfoDatas[x].vertexCount; z++)
					{
						float normalX;
						float normalY;
						float normalZ;
						float uvU;
						float uvV;
						if (stride == 32)
						{
							normalX = ReadFloat(f);
							normalY = ReadFloat(f);
							normalZ = ReadFloat(f);
							fseek(f, 4, SEEK_CUR);
							uvU = ReadHalfFloat(f);
							uvV = ReadHalfFloat(f);
							fseek(f, 12, SEEK_CUR);
						}
						else
						{
							normalX = ReadFloat(f);
							normalY = ReadFloat(f);
							normalZ = ReadFloat(f);
							uvU = ReadHalfFloat(f);
							uvV = ReadHalfFloat(f);
							fseek(f, stride - 16, SEEK_CUR);
						}

						normals.push_back(normalX);
						normals.push_back(normalY);
						normals.push_back(normalZ);
						uvs.push_back(uvU);
						uvs.push_back(-uvV);
					}
					fseek(f, vertexStartOffset + subInfoDatas[x].faceOffset + (subInfoDatas[x].previousFaceCount * 2), SEEK_SET);
					for (int v = 0; v < subInfoDatas[x].faceCount / 3; v++)
					{
						unsigned short faceA = ReadUShort(f);
						unsigned short faceB = ReadUShort(f);
						unsigned short faceC = ReadUShort(f);

						faces.push_back(faceA);
						faces.push_back(faceB);
						faces.push_back(faceC);
					}
				}
				FbxManager* lSdkManager = FbxManager::Create();
				FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
				FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
				lSdkManager->SetIOSettings(ios);
				int sumVertices = 0;
				int Viord = 0;
				int index = 0;
				int rememberI = 0;
				int remeberU = 0;
				int Niord = 0;
				for (int z = 0; z < SubInfoCount; z++)
				{
					FbxNode* lMeshNode = FbxNode::Create(lScene, "test");
					FbxMesh* lMesh = FbxMesh::Create(lScene, "test");
					lMeshNode->SetNodeAttribute(lMesh);
					FbxNode* lRootNode = lScene->GetRootNode();
					FbxNode* lPatchNode = lScene->GetRootNode();
					lRootNode->AddChild(lMeshNode);
					lMesh->InitControlPoints(subInfoDatas[z].vertexCount);
					FbxVector4* lControlPoints = lMesh->GetControlPoints();
					FbxLayer* lLayer = lMesh->GetLayer(0);
					if (lLayer == NULL) {
						lMesh->CreateLayer();
						lLayer = lMesh->GetLayer(0);
					}

					FbxLayerElementNormal* lLayerElementNormal = FbxLayerElementNormal::Create(lMesh, "");
					FbxLayerElementUV* lLayerElementUV = FbxLayerElementUV::Create(lMesh, "");
					// Set its mapping mode to map each normal vector to each control point.
					lLayerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);
					lLayerElementUV->SetMappingMode(FbxLayerElement::eByControlPoint);
					// Set the reference mode of so that the n'th element of the normal array maps to the n'th
					// element of the control point array.
					lLayerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);
					lLayerElementUV->SetReferenceMode(FbxLayerElement::eDirect);
					for (int j = 0; j < subInfoDatas[z].vertexCount; j++)
					{
						FbxVector4 vertex(vertices[Viord], vertices[Viord + 1], vertices[Viord + 2], vertices[Viord + 3]);
						FbxVector4 normal(normals[Niord], normals[Niord + 1], normals[Niord + 2]);
						FbxVector2 uv(uvs[remeberU], uvs[remeberU + 1]);
						lLayerElementNormal->GetDirectArray().Add(normal);
						lLayerElementUV->GetDirectArray().Add(uv);
						lControlPoints[j] = vertex;
						Viord += 4;
						Niord += 3;
						remeberU += 2;

					}
					for (int y = 0; y < subInfoDatas[z].faceCount / 3; y++)
					{
						lMesh->BeginPolygon();
						lMesh->AddPolygon(faces[rememberI]);
						lMesh->AddPolygon(faces[rememberI + 1]);
						lMesh->AddPolygon(faces[rememberI + 2]);
						lMesh->EndPolygon();
						rememberI += 3;

					}
					lLayer->SetNormals(lLayerElementNormal);
					lLayer->SetUVs(lLayerElementUV);
					lMeshNode->LclRotation.Set(FbxVector4(180, 0, 0.0)); //Right rotation

				}
				FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
				lSdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);
				std::string currentFileName = fns[i] + ".fbx";
				bool lExportStatus = lExporter->Initialize(currentFileName.c_str(), -1, lSdkManager->GetIOSettings());
				if (!lExportStatus) {
					throw gcnew System::Exception(gcnew System::String("Call to FbxExporter::Initialize() failed."));
				}
				lExporter->Export(lScene);
				lExporter->Destroy();
			}

			subInfoStarts.clear();
			subInfoDatas.clear();
			normalUVSize.clear();
		}
		else if (tagInfos[indices[i]].tag == "PTEX")
		{
			fseek(f, 100, SEEK_CUR);
			long ddsOffset = ReadLong(f);
			long ddsSize = ReadLong(f);
			fseek(f, dataInfos[dataInfos.size() - 1].dataOffset + ddsOffset, SEEK_SET);
			std::vector<unsigned char> dds;
			dds = ReadUBytes(f, ddsSize);
			FILE* fTex;

			std::string path = filename;

			int found = path.find_last_of("/\\");
			std::string folder = path.substr(0, found + 1);
			fseek(f, dataInfos[0].dataOffset + tagInfos[indices[i]].textOffset, SEEK_SET);
			std::string TextureName = ReadString(f);
			TextureName = TextureName.substr(0, TextureName.length() - 1);
			TextureName += ".dds";
			folder.append(TextureName);
			if (fopen_s(&fTex, folder.c_str(), "wb") != 0) // Security check
			{
				throw gcnew System::Exception(gcnew System::String("Can't write dds file"));
			}
			for (long i = 0; i < ddsSize; i++)
			{
				fwrite(&dds[i], 1, 1, fTex);
			}
			fclose(fTex);
		}
		else if (tagInfos[indices[i]].tag == "PCOL") //Havok stuff
		{
			fseek(f, 32, SEEK_CUR);
			long unknownCount = ReadLong(f);
			long havokFileInfoOffset = ReadLong(f);
			long havokFileOffset = ReadLong(f);
			long havokFileSize = ReadLong(f);

			FILE* fhavok;
			std::string currentHavokFileName = fns[i] + ".hkx"; //extension not sure
			if (fopen_s(&fhavok, currentHavokFileName.c_str(), "wb") != 0) // Security check
			{
				throw gcnew System::Exception(gcnew System::String("Can't write hkx file"));
			}
			fseek(f, dataInfos[1].dataOffset + havokFileOffset, SEEK_SET);
			std::vector<unsigned char> havokFile;
			havokFile = ReadUBytes(f, havokFileSize);
			for (long i = 0; i < havokFileSize; i++)
			{
				fwrite(&havokFile[i], 1, 1, fhavok);
			}
			fclose(fhavok);

		}
	}
	

}
