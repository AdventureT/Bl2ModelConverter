#include "trb2.h"

#include "TrbHeader.h"
#include "PMDL.h"
#include "PTEX.h"


trb2::trb2(std::string fileName, uint32_t type)
{
	this->filename = fileName;
	this->Type = type;
	if (fopen_s(&f, filename.c_str(), "rb+") != 0) // Security check
	{
		throw "Can't open the trb file";
	}
}

void trb2::readHeader()
{
	TrbHeader trbHeader(ReadString(f,4),ReadUInt(f), ReadUShort(f), ReadUShort(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f));
	fseek(f, 116, SEEK_CUR);
	uint32_t textOffset = ReadUInt(f);
	fseek(f, -28, SEEK_CUR);
	for (size_t i = 0; i < trbHeader.dataInfoCount; i++)
	{
		DataInfo dataInfo = { ReadUInt(f), StringOffset(ReadUInt(f), textOffset, f), ReadUInt(f) , ReadUInt(f) , ReadUInt(f),
		ReadUInt(f), ReadUInt(f), ReadUInt(f) , ReadUInt(f) , ReadUInt(f), ReadUInt(f), ReadUInt(f) };
		dataInfos.push_back(dataInfo);
	}
	long ret = ftell(f);
	fseek(f, dataInfos[0].dataOffset, SEEK_SET);
	if (ReadString(f, 4) == "BTEC")
	{
		System::Windows::Forms::MessageBox::Show("BTECs not supported"); return;
	}
	fseek(f, ret, SEEK_SET);
	for (size_t i = 0; i < trbHeader.tagCount; i++)
	{
		TagInfo tagInfo;
		if (ReadLong(f) == 0)
		{
			tagInfo = { "", ReadUInt(f), ReadUInt(f), ReadUInt(f) };
		}
		else
		{
			fseek(f, -4, SEEK_CUR);
			tagInfo = { ReadString(f,4), ReadUInt(f), ReadUInt(f), ReadUInt(f) };
			ret = ftell(f);
			fseek(f, tagInfo.textOffset + dataInfos[0].dataOffset, SEEK_SET);
			tagInfo.filename = ReadString(f);
			fseek(f, ret, SEEK_SET);
		}
		tagInfos.push_back(tagInfo);
	}
	fseek(f, dataInfos[0].dataOffset, SEEK_SET);
	while ((uint32_t)ftell(f) < dataInfos[0].dataOffset + dataInfos[0].dataSize)
	{
		text.push_back(ReadString(f));
	}
	fseek(f, trbHeader.relocationDataOffset, SEEK_SET);
	for (size_t i = 0; i < trbHeader.relocationDataSize / 12; i++)
	{
		SYMBInfo symbInfo = { ReadUShort(f), ReadUShort(f), ReadUShort(f),ReadUShort(f), ReadUInt(f) };
		symbInfos.push_back(symbInfo);
	}
}

std::vector<std::string> trb2::readListBoxInfos()
{
	std::vector<std::string> infos;
	for (size_t i = 0; i < tagInfos.size(); i++)
	{
		infos.push_back(tagInfos[i].tag); //tag
		fseek(f, tagInfos[i].textOffset + dataInfos[0].dataOffset, SEEK_SET);
		infos.push_back(ReadString(f)); //filename
		infos.push_back(std::to_string(i));
	}
	return infos;
}

std::string trb2::readData(std::vector<int> indices, std::vector<std::string> fns)
{
	std::string output;
	for (size_t i = 0; i < indices.size(); i++)
	{
		fseek(f, tagInfos[indices[i]].dataOffset + dataInfos[1].dataOffset, SEEK_SET);
		if (tagInfos[indices[i]].tag == "PMDL")
		{
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> uvs;
			std::vector<uint32_t> faces;
			std::vector<uint32_t> verticesCount;
			std::vector<uint32_t> facesCount;
			bool isSkeleton = true;
			PMDL pmdl;
			pmdl.symbolHeader = { ReadString(f, 4), ReadUInt(f), ReadUShort(f), ReadUShort(f), ReadUInt(f), ReadUInt(f), ReadInt(f), ReadUInt(f), ReadUInt(f) };
			fread(&pmdl.header, sizeof(PMDL::Header), 1, f);
			fread(&pmdl.coords, sizeof(PMDL::Coords), 1, f);
			fread(&pmdl.meshInfoTable, sizeof(PMDL::MeshInfoTable), 1, f);
			fseek(f, pmdl.header.boneTableOffset + dataInfos[1].dataOffset, SEEK_SET);
			fread(&pmdl.boneTable, sizeof(PMDL::BoneTable), 1, f);
			if (pmdl.boneTable.boneCount == 0)
			{
				isSkeleton = false;
			}
			fseek(f, pmdl.meshInfoTable.meshInfoOffsetsOffset + dataInfos[1].dataOffset, SEEK_SET);
			for (size_t j = 0; j < pmdl.meshInfoTable.meshInfoCount; j++)
			{
				pmdl.meshInfoOffsets.push_back(ReadUInt(f));
			}
			for (size_t b = 0; b < pmdl.meshInfoTable.meshInfoCount; b++)
			{
				fseek(f, pmdl.meshInfoOffsets[b] + dataInfos[1].dataOffset, SEEK_SET);
				PMDL::MeshInfo meshInfo;
				fread(&meshInfo, sizeof(PMDL::MeshInfo), 1, f);
				pmdl.meshInfos.push_back(meshInfo);
				verticesCount.push_back(meshInfo.vertexCount);
				facesCount.push_back(meshInfo.faceCount);
			}
			uint32_t normalUVStart = 0;
			uint32_t normalUVEnd = 0;
			std::vector<uint32_t> normalUVSize;
			if (pmdl.meshInfoTable.meshInfoCount > 1)
			{
				int remember = 0;
				for (size_t a = 0; a + 1 < pmdl.meshInfoTable.meshInfoCount; a++)
				{
					normalUVStart = pmdl.meshInfos[a].normalUVOffset;
					normalUVEnd = pmdl.meshInfos[a + 1].normalUVOffset;
					normalUVSize.push_back(normalUVEnd - normalUVStart);
					remember = a;
				}
				normalUVStart = pmdl.meshInfos[remember + 1].normalUVOffset;
				normalUVEnd = pmdl.meshInfos[remember + 1].faceOffset;
				normalUVSize.push_back(normalUVEnd - normalUVStart);
			}
			else if (pmdl.meshInfoTable.meshInfoCount == 1)
			{
				normalUVStart = pmdl.meshInfos[0].normalUVOffset;
				normalUVEnd = pmdl.meshInfos[0].faceOffset;
				normalUVSize.push_back(normalUVEnd - normalUVStart);
			}
			if (pmdl.meshInfoTable.meshInfoCount >= 1)
			{
				for (size_t x = 0; x < pmdl.meshInfoTable.meshInfoCount; x++)
				{
					int stride = normalUVSize[x] / pmdl.meshInfos[x].vertexCount;
					fseek(f, pmdl.header.vertexStartOffset + dataInfos[2].dataOffset + pmdl.meshInfos[x].vertexOffsetRelative, SEEK_SET);
					for (size_t j = 0; j < pmdl.meshInfos[x].vertexCount; j++)
					{
						vertices.push_back(ReadHalfFloat(f));
						vertices.push_back(ReadHalfFloat(f));
						vertices.push_back(ReadHalfFloat(f));
						vertices.push_back(ReadHalfFloat(f));
					}
					fseek(f, pmdl.header.vertexStartOffset + dataInfos[2].dataOffset + pmdl.meshInfos[x].normalUVOffset, SEEK_SET);
					for (size_t z = 0; z < pmdl.meshInfos[x].vertexCount; z++)
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
					fseek(f, pmdl.header.vertexStartOffset + dataInfos[2].dataOffset + pmdl.meshInfos[x].faceOffset + (pmdl.meshInfos[x].previousFaceCount * 2), SEEK_SET);
					for (size_t v = 0; v < pmdl.meshInfos[x].faceCount / 3; v++)
					{
						faces.push_back(ReadUShort(f));
						faces.push_back(ReadUShort(f));
						faces.push_back(ReadUShort(f));
					}
				}
				FbxHelper::Model m = { vertices,faces,normals,uvs,verticesCount,facesCount,pmdl.meshInfoTable.meshInfoCount, "test" };
				FbxHelper fbxH;
				if (isSkeleton)
				{
					std::vector<float> matrices;
					std::vector<std::vector<double>> TRS;
					std::vector<FbxAMatrix> matlocals;
					std::vector<short> parents;
					std::vector<std::string> boneNames;
					fseek(f, pmdl.boneTable.bonesOffset + dataInfos[1].dataOffset, SEEK_SET);
					for (size_t i = 0; i < pmdl.boneTable.boneCount; i++)
					{
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						matrices.push_back(ReadFloat(f));
						double T[3];
						double R[3];
						double S[3];
						FbxHelper::Matrix344ToTRS(matrices.data(), T, R, S, 0);
						FbxAMatrix matLocal;
						matLocal.SetRow(0, FbxVector4(matrices[0], matrices[1], matrices[2], matrices[3]));
						matLocal.SetRow(1, FbxVector4(matrices[4], matrices[5], matrices[6], matrices[7]));
						matLocal.SetRow(2, FbxVector4(matrices[8], matrices[9], matrices[10], matrices[11]));
						matLocal.SetRow(3, FbxVector4(matrices[12], matrices[13], matrices[14], matrices[15]));
						matlocals.push_back(matLocal);
						std::vector<double> trs;
						trs.push_back(matrices[12]); trs.push_back(matrices[13]); trs.push_back(matrices[14]);
						trs.push_back(R[0]); trs.push_back(R[1]); trs.push_back(R[2]);
						trs.push_back(S[0]); trs.push_back(S[1]); trs.push_back(S[2]);
						TRS.push_back(trs);
						matrices.clear();
					}
					fseek(f, pmdl.boneTable.boneNamesOffset + dataInfos[1].dataOffset, SEEK_SET);
					for (size_t i = 0; i < pmdl.boneTable.boneCount; i++)
					{
						
						uint32_t boneNameOffset = ReadUInt(f);
						long retHere = ftell(f);
						fseek(f, boneNameOffset + dataInfos[0].dataOffset, SEEK_SET);
						boneNames.push_back(ReadString(f));
						fseek(f, retHere, SEEK_SET);
					}
					fseek(f, pmdl.boneTable.boneParentsOffset + dataInfos[1].dataOffset, SEEK_SET);
					for (size_t i = 0; i < pmdl.boneTable.boneCount; i++)
					{
						parents.push_back(ReadUShort(f));
						output += "Bone ";
						output += std::to_string(i + 1);
						output += ": ";
						output += boneNames[i];
						output += " Parent: ";
						output += std::to_string(parents[i]);
						output += "\n";
					}
					fbxH.CreateSkeleton(boneNames, parents, TRS, matlocals);
				}
				fbxH.CreateFbx(m, fns[i]);
			}
			subInfoStarts.clear();
			subInfoDatas.clear();
			normalUVSize.clear();
		}
		else if (tagInfos[indices[i]].tag == "PTEX")
		{
			PTEX ptex;
			ptex.symbolHeader = { ReadString(f, 4), ReadUInt(f), ReadUShort(f), ReadUShort(f), ReadUInt(f), ReadUInt(f), ReadInt(f), ReadUInt(f), ReadUInt(f) };
			fread(&ptex.texInfo, sizeof(PTEX::TextureInfo), 1, f);
			fseek(f, dataInfos[dataInfos.size() - 1].dataOffset + ptex.texInfo.ddsOffset, SEEK_SET);
			std::vector<uint8_t> dds;
			dds = ReadBytes(f, ptex.texInfo.ddsSize);
			FILE* fTex;

			std::string path = filename;

			int found = path.find_last_of("/\\");
			std::string folder = path.substr(0, found + 1);
			fseek(f, dataInfos[0].dataOffset + tagInfos[indices[i]].textOffset, SEEK_SET);
			std::string TextureName = ReadString(f);
			TextureName = TextureName.substr(0, TextureName.length()).append(".dds");
			folder.append(TextureName);
			if (fopen_s(&fTex, folder.c_str(), "wb") != 0) // Security check
			{
				throw "Can't write dds file";
			}
			fwrite(&dds, ptex.texInfo.ddsSize, 1, fTex);
			fclose(fTex);
		}
		else if (tagInfos[indices[i]].tag == "PCOL") //Collosion (Havok file + Model Data)
		{
			std::string label = ReadString(f, 4);
			uint32_t zero = ReadUInt(f);
			uint16_t relocationDataCount = ReadUShort(f); //?? Unsure
			uint16_t pcolSize = ReadUShort(f);
			zero = ReadLong(f);
			uint32_t collosionTextOffset = ReadUInt(f);
			fseek(f, 12, SEEK_CUR);
			uint32_t collosionModelInfoCount = ReadUInt(f);
			uint32_t havokFileInfoOffset = ReadUInt(f);
			uint32_t havokFileOffset = ReadUInt(f);
			uint32_t havokFileSize = ReadUInt(f);

			fseek(f, havokFileInfoOffset + dataInfos[1].dataOffset, SEEK_SET);

			std::vector<SubCollsionInfo> subcols;
			std::vector<uint32_t> verticesCount;
			std::vector<uint32_t> facesCount;
			for (size_t i = 0; i < collosionModelInfoCount; i++)
			{
				subcols.push_back({ ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f) });
				verticesCount.push_back(subcols[i].vertCount / 3);
				facesCount.push_back(subcols[i].faceCount);
			}

			std::vector<float> vertices;
			std::vector<uint32_t> faces;

			for (size_t i = 0; i < collosionModelInfoCount; i++)
			{
				fseek(f, subcols[i].vertOffset + dataInfos[1].dataOffset, SEEK_SET);
				for (size_t j = 0; j < subcols[i].vertCount; j++)
				{
					vertices.push_back(ReadFloat(f));
				}

				fseek(f, subcols[i].faceOffset + dataInfos[1].dataOffset, SEEK_SET);
				for (size_t j = 0; j < subcols[i].faceCount; j++)
				{
					faces.push_back(ReadUInt(f));
				}
			}

			FbxHelper::Model m = { vertices,faces,std::vector<float>(),std::vector<float>(),verticesCount,facesCount,collosionModelInfoCount, "test" };
			FbxHelper fbxH;
			fbxH.CreateFbx(m, fns[i]);

			FILE* fhavok;
			std::string currentHavokFileName = fns[i] + ".hkx"; //extension not sure
			if (fopen_s(&fhavok, currentHavokFileName.c_str(), "wb") != 0) // Security check
			{
				throw gcnew System::Exception(gcnew System::String("Can't write hkx file"));
			}
			fseek(f, dataInfos[1].dataOffset + havokFileOffset, SEEK_SET);
			std::vector<unsigned char> havokFile;
			havokFile = ReadBytes(f, havokFileSize);
			for (size_t i = 0; i < havokFileSize; i++)
			{
				fwrite(&havokFile[i], 1, 1, fhavok);
			}
			fclose(fhavok);

		}
		else if (tagInfos[indices[i]].tag == "enti") //Entities
		{
			//ReadProperties();
			
		}
		else if (tagInfos[indices[i]].tag == "PMML") //Something to do with models and materials
		{
			long tell = ftell(f);
			std::string label = ReadString(f, 4);
			long zero = ReadLong(f);
			long relocationDataCount = ReadLong(f); //?? Unsure
			zero = ReadLong(f);
			long pmmlTextOffset = ReadLong(f);
		}
		else if (tagInfos[indices[i]].tag == "PMST") //??
		{
			std::string label = ReadString(f, 4);
			long zero = ReadLong(f);
			long relocationDataCount = ReadLong(f); //?? Unsure
			zero = ReadLong(f);
			long pmstTextOffset = ReadLong(f);
			fseek(f, 12, SEEK_CUR);
			long unknownCount = ReadLong(f);
		}
		else if (tagInfos[indices[i]].tag == "tskl") //Toshi Skeleton
		{
			std::string label = ReadString(f, 4);
			long zero = ReadLong(f);
			long relocationDataCount = ReadLong(f); //?? Unsure
			zero = ReadLong(f);
			long skeletonTextOffset = ReadLong(f);
			fseek(f, 28, SEEK_CUR);
			long boneCount = ReadLong(f);
			long noClue = ReadLong(f);
			long matricesStartOffset = ReadLong(f);
			zero = ReadLong(f);
			long secondSectionOffset = ReadLong(f);
			fseek(f, 20, SEEK_CUR);
			long boneNamesOffset = ReadLong(f);
			std::vector<std::string> boneNames;
			fseek(f, dataInfos[1].dataOffset + matricesStartOffset, SEEK_SET);
			std::vector<float> matrices;
			std::vector<std::vector<double>> TRS;
			std::vector<FbxAMatrix> matlocals;
			std::vector<short> parents;
			FbxQuaternion rotation(ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f));
			float basePose[] = { ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f) };
			float inverseBasePose[] = { ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f),
				ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f) };
			FbxVector4 translation(ReadFloat(f), ReadFloat(f), ReadFloat(f), ReadFloat(f));
			StringOffset boneName(ReadUInt(f), dataInfos[1].dataOffset, f);
			int16_t parentBoneID = ReadShort(f);
			/*for (int i = 0; i < boneCount; i++)
			{
				fseek(f, 16, SEEK_CUR);
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				matrices.push_back(ReadFloat(f));
				fseek(f, 80, SEEK_CUR);
				long boneNameOffset = ReadLong(f);
				short parent = ReadShort(f);
				int retHere = ftell(f);
				fseek(f, dataInfos[1].dataOffset + boneNameOffset, SEEK_SET);
				boneNames.push_back(ReadString(f));
				parents.push_back(parent);
				fseek(f, retHere, SEEK_SET);
				output += "Bone ";
				output += std::to_string(i + 1);
				output += ": ";
				output += boneNames[i].erase(boneNames[i].size() - 1);
				output += " Parent: ";
				output += std::to_string(parent);
				output += "\n";
				fseek(f, 10, SEEK_CUR);
				double T[3];
				double R[3];
				double S[3];
				FbxHelper::Matrix344ToTRS(matrices.data(),T,R,S,0);
				FbxAMatrix matLocal;
				matLocal.SetRow(0, FbxVector4(matrices[0], matrices[1], matrices[2], matrices[3]));
				matLocal.SetRow(1, FbxVector4(matrices[4], matrices[5], matrices[6], matrices[7]));
				matLocal.SetRow(2, FbxVector4(matrices[8], matrices[9], matrices[10], matrices[11]));
				matLocal.SetRow(3, FbxVector4(matrices[12], matrices[13], matrices[14], matrices[15]));
				matlocals.push_back(matLocal);
				std::vector<double> trs;
				trs.push_back(T[0]); trs.push_back(T[1]); trs.push_back(T[2]);
				trs.push_back(R[0]); trs.push_back(R[1]); trs.push_back(R[2]);
				trs.push_back(S[0]); trs.push_back(S[1]); trs.push_back(S[2]);
				TRS.push_back(trs);
				matrices.clear();
			}*/
			//FbxHelper fbxH;
			//fbxH.CreateSkeleton(boneNames, parents, TRS, matlocals);
		}
		else if (tagInfos[indices[i]].tag == "tcmt") //Toshi Core Material
		{
			long pos = ftell(f);
			long offsetOrSize = ReadLong(f);
		}
		else if (tagInfos[indices[i]].tag == "tcmd") //Toshi Core Model
		{
			long pos = ftell(f);
		}
		else if (tagInfos[indices[i]].tag == "tseq")
		{
			long pos = ftell(f);
			long offsetOrSize = ReadLong(f);
		}
		else if (tagInfos[indices[i]].tag == "")
		{
			fseek(f, tagInfos[i].textOffset + dataInfos[0].dataOffset, SEEK_SET);
			if (ReadString(f,4) == "scor") //Scoring.trb
			{
				fseek(f, tagInfos[indices[i]].dataOffset + dataInfos[1].dataOffset, SEEK_SET);
				scoringInfoOffset = ReadLong(f);
				long scoringInfoCount = ReadLong(f);
				fseek(f, scoringInfoOffset + dataInfos[1].dataOffset, SEEK_SET);
				for (int i = 0; i < scoringInfoCount; i++)
				{
					uint32_t scoringNameOffset = ReadUInt(f);
					long retHere = ftell(f);
					fseek(f, scoringNameOffset + dataInfos[1].dataOffset, SEEK_SET);
					std::string scoringName = ReadString(f);
					fseek(f, retHere, SEEK_SET);
					uint32_t unknown = ReadUInt(f);
					float value = ReadFloat(f);
					si.push_back({scoringName, scoringNameOffset, unknown, value});
					output += si[i].scoringName.erase(si[i].scoringName.size() - 1);
					output += " Value: ";
					output += std::to_string(si[i].value);
					output += "\n";
				}
				long offset1 = ReadLong(f);
				offset1 = ReadLong(f);
				offset1 = ReadLong(f);
			}
		}
	}
	return output;
}
void trb2::writeScoringData(float scoringValue, int index)
{
	fseek(f, scoringInfoOffset + dataInfos[1].dataOffset + index * 12 + 8, SEEK_SET);
	fwrite(&scoringValue, 4, 1, f);
}

std::vector<std::wstring> trb2::ReadLocaleStrings()
{
	fseek(f, tagInfos[0].dataOffset + dataInfos[1].dataOffset, SEEK_SET);
	uint32_t stringCount = ReadUInt(f);
	uint32_t stringsOffset = ReadUInt(f);
	fseek(f, dataInfos[1].dataOffset + stringsOffset, SEEK_SET);
	std::vector<std::wstring> localeStrings;
	for (size_t i = 0; i < stringCount; i++)
	{
		uint32_t stringOffset = ReadUInt(f);
		long rt = ftell(f);
		fseek(f, stringOffset + dataInfos[1].dataOffset, SEEK_SET);
		uint16_t c = ReadUShort(f);
		fseek(f, -2, SEEK_CUR);
		if (c > 0xFF) // ASCII
		{
			//TODO Read String
			std::string localeString = ReadString(f);
			localeStrings.push_back(std::wstring(localeString.begin(), localeString.end()));
		}
		else // Unicode
		{
			//TODO Read UnicodeString
			localeStrings.push_back(ReadUnicodeString(f));
		}
		fseek(f, rt, SEEK_SET);
	}
	return localeStrings;
}

struct trb2::Props trb2::ReadProperties()
{
	fseek(f, tagInfos[0].dataOffset + dataInfos[1].dataOffset, SEEK_SET);
	uint32_t entitiesInfoOffset = ReadUInt(f);
	uint32_t entitiesInfoCount = ReadUInt(f);
	fseek(f, entitiesInfoOffset + dataInfos[1].dataOffset, SEEK_SET);
	std::vector<EntitiesInfo> entitiesInfos;
	struct Props prop;
	for (size_t i = 0; i < entitiesInfoCount; i++)
	{
		entitiesInfos.push_back({ ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f), ReadUInt(f) });
	}
	for (size_t i = 0; i < entitiesInfoCount; i++)
	{
		fseek(f, entitiesInfos[i].entityNameOffset + dataInfos[0].dataOffset, SEEK_SET);
		prop.entityNames.push_back(ReadString(f));
	}
	//Read Properties
	for (size_t i = 0; i < entitiesInfos[i].propertiesCount; i++)
	{
		fseek(f, entitiesInfos[i].propertiesOffset + dataInfos[1].dataOffset, SEEK_SET);
		std::vector<std::string> propNamess;
		for (size_t j = 0; j < entitiesInfos[i].propertiesCount; j++)
		{	
			Property p = {ReadUInt(f), static_cast<PrimitiveType>(ReadUInt(f)), ReadUInt(f)};
			long returnH = ftell(f);
			fseek(f, p.propertyNameOffset + dataInfos[0].dataOffset, SEEK_SET);
			propNamess.push_back(ReadString(f));
			fseek(f, returnH, SEEK_SET);
		}
		prop.propNames.push_back(propNamess);
		//TODO
	}
	return prop;
}



trb2::~trb2()
{
	// Nothing to delete right now
}


