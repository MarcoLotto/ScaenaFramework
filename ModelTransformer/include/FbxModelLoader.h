/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#define FBXSDK_NEW_API
#include <fbxsdk.h>
#include "ModelDataTransfer.h"
#include <vector>
using namespace std;

class Mesh;

class FbxModelLoader{

private:
	FbxManager* lSdkManager;
	int jointIndexCount;
	FbxScene* lScene;
	
	ModelDataTransfer* processContent(FbxScene* pScene);
	void processContent(FbxNode* pNode, int iteration, MeshDataTransfer* meshDataTransfer, SkeletonDataTransfer* skeletonDataTransfer);
	MeshDataTransfer* processMesh(FbxNode* pNode, SkeletonDataTransfer* skeletonDataTransfer);	
	SkeletonDataTransfer* processSkeleton(FbxNode* pNode, SkeletonDataTransfer* parentSkeletonDataTransfer);
	void processLinks(FbxNode* pNode, MeshDataTransfer* meshDataTransfer, SkeletonDataTransfer* skeletonDataTransfer);
	bool filterUnbindedSkeletonJoints(SkeletonDataTransfer* skeletonJoint, SkeletonDataTransfer* jointParent);
	void processAnimations(SkeletonDataTransfer* skeletonJoint, FbxNode* pNode, FbxCluster* lCluster);

public:
	FbxModelLoader();
	virtual ~FbxModelLoader();

	// Carga un archivo fbx de disco y lo interpreta en un modeldatatransfer
	ModelDataTransfer* loadModel(const char* filename);

};