#pragma once
#include <fbxsdk.h>
#include <vector>
public class FbxHelper
{
public:

	struct Model
	{
		std::vector<float> vertices;
		std::vector<unsigned long> faces;
		std::vector<float> normals;
		std::vector<float> uvs;
		std::vector<long> vertCount;
		std::vector<long> faceCount;
		int meshCount;
		std::string meshName;
	};

	void CreateFbx(Model m, std::string fileName);

};

