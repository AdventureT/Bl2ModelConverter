#include "FbxHelper.h"

void FbxHelper::CreateFbx(Model m, std::string fileName)
{
	bool noNormals = false;
	bool noUvs = false;
	int sumVertices = 0;
	int Viord = 0;
	int index = 0;
	int rememberI = 0;
	int remeberU = 0;
	int Niord = 0;

	if (m.normals.empty())
	{
		noNormals = true;
	}
	else if (m.uvs.empty())
	{
		noUvs = true;
	}
	for (int i = 0; i < m.meshCount; i++)
	{
		FbxManager* lSdkManager = FbxManager::Create();
		FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");
		FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);
		FbxNode* lMeshNode = FbxNode::Create(lScene, m.meshName.c_str());
		FbxMesh* lMesh = FbxMesh::Create(lScene, m.meshName.c_str());
		lMeshNode->SetNodeAttribute(lMesh);
		FbxNode* lRootNode = lScene->GetRootNode();
		FbxNode* lPatchNode = lScene->GetRootNode();
		lRootNode->AddChild(lMeshNode);
		lMesh->InitControlPoints(m.vertCount[i]);
		FbxVector4* lControlPoints = lMesh->GetControlPoints();
		FbxLayer* lLayer = lMesh->GetLayer(0);
		FbxLayerElementNormal* lLayerElementNormal;
		FbxLayerElementUV* lLayerElementUV;
		if (lLayer == NULL) {
			lMesh->CreateLayer();
			lLayer = lMesh->GetLayer(0);
		}
		if (!m.normals.empty())
		{
			lLayerElementNormal = FbxLayerElementNormal::Create(lMesh, "");
			lLayerElementNormal->SetMappingMode(FbxLayerElement::eByControlPoint);
			lLayerElementNormal->SetReferenceMode(FbxLayerElement::eDirect);
		}
		if (!m.uvs.empty())
		{
			lLayerElementUV = FbxLayerElementUV::Create(lMesh, "");
			lLayerElementUV->SetMappingMode(FbxLayerElement::eByControlPoint);
			lLayerElementUV->SetReferenceMode(FbxLayerElement::eDirect);
		}


		for (int j = 0; j < m.vertCount[i]; j++)
		{
			FbxVector4 vertex(m.vertices[Viord], m.vertices[Viord + 1], m.vertices[Viord + 2]);
			if (!m.normals.empty())
			{
				FbxVector4 normal(m.normals[Niord], m.normals[Niord + 1], m.normals[Niord + 2]);
				lLayerElementNormal->GetDirectArray().Add(normal);
			}
			if (!m.uvs.empty())
			{
				FbxVector2 uv(m.uvs[remeberU], m.uvs[remeberU + 1]);
				lLayerElementUV->GetDirectArray().Add(uv);
			}
			lControlPoints[j] = vertex;
			if (m.vertices[Viord + 3] == 1)
			{
				Viord += 4;
			}
			else
			{
				Viord += 3;
			}
			Niord += 3;
			remeberU += 2;
		}
		for (int y = 0; y < m.faceCount[i] / 3; y++)
		{
			lMesh->BeginPolygon();
			lMesh->AddPolygon(m.faces[rememberI]);
			lMesh->AddPolygon(m.faces[rememberI + 1]);
			lMesh->AddPolygon(m.faces[rememberI + 2]);
			lMesh->EndPolygon();
			rememberI += 3;
		}
		if (!m.normals.empty())
		{
			lLayer->SetNormals(lLayerElementNormal);
		}
		if (!m.uvs.empty())
		{
			lLayer->SetUVs(lLayerElementUV);
		}
		lMeshNode->LclRotation.Set(FbxVector4(180, 0, 0.0)); //Right rotation

		FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
		lSdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, true);
		std::string currentFileName = fileName + ".fbx";
		bool lExportStatus = lExporter->Initialize(currentFileName.c_str(), -1, lSdkManager->GetIOSettings());
		if (!lExportStatus) {
			throw gcnew System::Exception(gcnew System::String("Call to FbxExporter::Initialize() failed."));
		}
		lExporter->Export(lScene);
		lExporter->Destroy();
	}
	
}
