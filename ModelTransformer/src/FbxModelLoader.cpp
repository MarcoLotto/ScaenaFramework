/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "FbxModelLoader.h"
#include <fbxsdk.h>
#include "ModelDataTransfer.h"
#include "Logger.h"
#include "ImportUtilities.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include "KeyFrameDataTransfer.h"
#include <vector>
using namespace std;
typedef glm::vec4 vec4;

#define DIFFUSE_TEXTURES_PATH "./Textures/Difusas/"
#define NORMAL_TEXTURES_PATH "./Textures/NormalMaps/"
#define DISPLACEMENT_TEXTURES_PATH "./Textures/DisplacementMaps/"

FbxModelLoader::FbxModelLoader(){
	// Initialize the sdk manager. This object handles all our memory management.
	 this->lSdkManager = FbxManager::Create();

	 // Create the io settings object.    
	 FbxIOSettings *ios = FbxIOSettings::Create(this->lSdkManager, IOSROOT);   
	 this->lSdkManager->SetIOSettings(ios);	

	 this->jointIndexCount = 0;
}

FbxModelLoader::~FbxModelLoader(){
	this->lSdkManager->Destroy();
}

ModelDataTransfer* FbxModelLoader::loadModel(const char* filename){

	 // Create an importer using our sdk manager.    
	 FbxImporter* lImporter = FbxImporter::Create(this->lSdkManager, "importer1");

	 // Use the first argument as the filename for the importer.    
	 if(!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings())) {        
		 printf("Call to FbxImporter::Initialize() failed.\n");        
		 //printf("Error returned: %s\n\n", lImporter->GetLastErrorString()); 
		 return false;
	}

	 // Create a new scene so it can be populated by the imported file.    
	 this->lScene = FbxScene::Create(lSdkManager,"myScene");    
	 // Import the contents of the file into the scene.    
	 lImporter->Import(this->lScene);    
	 // The file has been imported; we can get rid of the importer.   
	 lImporter->Destroy();

	 // Consigo todos los datos del fbx  
	 ModelDataTransfer* modelDataTransfer = this->processContent(this->lScene);
	 modelDataTransfer->filepath = filename;

	 return modelDataTransfer;
}

ModelDataTransfer* FbxModelLoader::processContent(FbxScene* pScene)
{   
	Logger::getInstance()->logInfo(new Log("Procesando el FBX de entrada..."));			

	MeshDataTransfer* rootMeshDt = new MeshDataTransfer();
	rootMeshDt->name = "root";
	rootMeshDt->hasSkiningInfo = false;

	SkeletonDataTransfer* rootSkeletonDt = new SkeletonDataTransfer();
	rootSkeletonDt->jointName = "root";	

	// Mando a procesar todos los nodos del FBX
    FbxNode* lNode = pScene->GetRootNode();
    if(lNode)
    {
		// Hago dos iteraciones, en la primera proceso el skeleton, en la segunda la geometria y relaciones con el esqueleto
		for(int iteration = 0; iteration < 2; iteration++){
			for(int i = 0; i < lNode->GetChildCount(); i++)
			{
			    this->processContent(lNode->GetChild(i), iteration, rootMeshDt, rootSkeletonDt);
			}
		}
    }
	// Si hay esqueleto, filtro aquellos joints que no estan bindieados con ningun mesh
	if(rootSkeletonDt->getChilds()->size() > 0){
		this->jointIndexCount = 0;
		this->filterUnbindedSkeletonJoints(rootSkeletonDt, NULL);
	}

	// Armo el data transfer final, agregando cada componente
	ModelDataTransfer* modelDataTransfer = new ModelDataTransfer();
	if(rootMeshDt->getChilds()->size() > 0)
		modelDataTransfer->setRootMesh(rootMeshDt);
	else
		delete rootMeshDt;
	if(rootSkeletonDt->getChilds()->size() > 0)
		modelDataTransfer->setRootSkeleton(rootSkeletonDt);
	else
		delete rootSkeletonDt;

	return modelDataTransfer;
}

void FbxModelLoader::processContent(FbxNode* pNode, int iteration, MeshDataTransfer* meshDataTransfer, SkeletonDataTransfer* skeletonDataTransfer)
{
	MeshDataTransfer* meshChildDataTransfer = NULL;
	SkeletonDataTransfer* skeletonChildDataTransfer = NULL;

    FbxNodeAttribute::EType lAttributeType;
    if(pNode->GetNodeAttribute() != NULL)
    {  
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());		
        switch (lAttributeType)
        {
        case FbxNodeAttribute::eMarker:  
            //childDataTransfer = DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
			if(iteration == 0){  // Primera iteracion, solo proceso esqueleto				
				skeletonChildDataTransfer = this->processSkeleton(pNode, skeletonDataTransfer);
				skeletonDataTransfer->addChild(skeletonChildDataTransfer);
			}
            break;

        case FbxNodeAttribute::eMesh:     
			if(iteration == 1){  // Recien en la segunda iteracion proceso geometria (preciso antes el esqueleto)				
				meshChildDataTransfer = this->processMesh(pNode, skeletonDataTransfer);	
				meshDataTransfer->addChild(meshChildDataTransfer);
			}
            break;

        case FbxNodeAttribute::eNurbs:      
            //childDataTransfer = DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            //childDataTransfer = DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            //childDataTransfer = DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            //childDataTransfer = DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            //childDataTransfer = DisplayLodGroup(pNode);
            break;
        }   
    }    

	// Proceso cada uno de los hijos
	if(meshChildDataTransfer || skeletonChildDataTransfer){
		if(meshChildDataTransfer == NULL)
			meshChildDataTransfer = meshDataTransfer;
		if(skeletonChildDataTransfer == NULL)
			skeletonChildDataTransfer = skeletonDataTransfer;	

		for(int i = 0; i < pNode->GetChildCount(); i++)
		{
			this->processContent(pNode->GetChild(i), iteration, meshChildDataTransfer, skeletonChildDataTransfer);
		}		
	}
}

glm::vec3 toVector(FbxVector4 vector){
	return glm::vec3(vector[0], vector[1], vector[2]);
}
glm::vec3 toVector(FbxDouble3 vector){
	return glm::vec3(vector[0], vector[1], vector[2]);
}
glm::vec2 toVector(FbxDouble2 vector){
	return glm::vec2(vector[0], vector[1]);
}

//Recibe un path absoluto (ej. de una textura) y lo transforma en un path relativo a partir de la subruta
char* getRelativePath(const char* subRute, const char* absolutePath){	
	int largo = strlen(absolutePath);
	int posCorte = 0;
	char* relativePath = new char[largo];
	for(int i=largo; i > 0 ; i--){
		if(absolutePath[i] == '\\')
			break;
		relativePath[largo - i] = absolutePath[i];
		posCorte++;
	}
	posCorte--;
	//invierto el arreglo
	for (int i=0; i < posCorte/2 + 1 ; i++){
		//swap
		char aux = relativePath[i];
		relativePath[i] = relativePath[posCorte - i];
		relativePath[posCorte - i] = aux;
	}
	//Agrego el subRute
	int subRuteLength = strlen(subRute);
	char* result = new char[subRuteLength+posCorte+1];
	for(int i=0; i < subRuteLength; i++)
		result[i] = subRute[i];
	for(int i=subRuteLength; i < posCorte+subRuteLength; i++)
		result[i] = relativePath[i - subRuteLength];
	result[posCorte+subRuteLength] = 0;
	delete(relativePath);
	return result;
}

string processTexture(FbxNode* pNode, glm::vec2 *UVtranslation, glm::vec2 *UVscale){

	string filename;
	FbxTexture* lTexture;	
	FbxProperty lProperty;
	int lNbMat = pNode->GetSrcObjectCount(FbxSurfaceMaterial::ClassId);	
	for (int lMaterialIndex = 0; lMaterialIndex < lNbMat; lMaterialIndex++)
	{
		FbxSurfaceMaterial *lMaterial = (FbxSurfaceMaterial *)pNode->GetSrcObject(FbxSurfaceMaterial::ClassId, lMaterialIndex);		
	
		if(lMaterial)
		{		
			lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
			lTexture = FbxCast <FbxTexture> (lProperty.GetSrcObject(FbxTexture::ClassId, 0));	
			FbxFileTexture* fbxFileTexture = FbxCast<FbxFileTexture>(lTexture);
			if(fbxFileTexture){         
				const char* name = fbxFileTexture->GetFileName();					
				filename = getRelativePath(DIFFUSE_TEXTURES_PATH, name);
				*UVscale = glm::vec2(lTexture->GetScaleU(), lTexture->GetScaleV());
				*UVtranslation = glm::vec2(lTexture->GetTranslationU(), lTexture->GetTranslationV());
				lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[0]);				
			}
		}		
	}
	return filename;
}

//Devuelve una textura de normal map ya cargada. Si no hay ninguna textura de normal map, devuelve NULL.
string processNormalMap(FbxNode* pNode){	
	FbxTexture* lTexture;
	//Busco normal maps
	FbxSurfaceMaterial* pMat = pNode->GetMaterial( 0 );
	FbxProperty oProperty = pMat->FindProperty(FbxSurfaceMaterial::sNormalMap);
	
	lTexture = FbxCast <FbxTexture> (oProperty.GetSrcObject(FbxTexture::ClassId,0));
	FbxFileTexture* fbxFileTexture = FbxCast<FbxFileTexture>(lTexture);
	if(fbxFileTexture == NULL)
		return "";	

	const char* name = fbxFileTexture->GetFileName();
	return getRelativePath(NORMAL_TEXTURES_PATH, name);
}

//Devuelve una textura de heigth map ya cargada. Si no hay ninguna textura de normal map, devuelve NULL.
string processDisplacementMap(FbxNode* pNode){	
	FbxTexture* lTexture;
	//Busco normal maps
	FbxSurfaceMaterial* pMat = pNode->GetMaterial( 0 );
	FbxProperty oProperty = pMat->FindProperty(FbxSurfaceMaterial::sDisplacementColor);
	

	lTexture = FbxCast <FbxTexture> (oProperty.GetSrcObject(FbxTexture::ClassId,0));
	FbxFileTexture* fbxFileTexture = FbxCast<FbxFileTexture>(lTexture);
	if(fbxFileTexture == NULL)
		return "";
	
	const char* name = fbxFileTexture->GetFileName();
	return getRelativePath(DISPLACEMENT_TEXTURES_PATH, name);
}


void loadFactors(MeshDataTransfer *mdt, FbxNode* pNode){
	FbxSurfaceMaterial* pMat = pNode->GetMaterial( 0 );
	mdt->displacementFactor = ((FbxSurfacePhong *)pMat)->BumpFactor.Get();
}


float* processUV(FbxMesh* fbxMesh, int* count, glm::vec2 *UVtranslation, glm::vec2 *UVscale){
	float* uvBuffer;
	FbxLayer *uvLayer = fbxMesh->GetLayer(0, FbxLayerElement::eUV);
	if(uvLayer != NULL){
		FbxLayerElementUV *uvElement = uvLayer->GetUVs();
		FbxLayerElementArrayTemplate<FbxVector2> *uvArray = &(uvElement->GetDirectArray());
		int uvCount = uvArray->GetCount();

		uvBuffer = new float[2 * uvCount];

		for (int i=0;i<uvCount;i++){
			//Aplico tambien la transformacion y escala
			int index = 2*i;
			glm::vec2 vertice2D = toVector(uvArray->GetAt(i));
			uvBuffer[index] = (vertice2D.x + UVtranslation->x) * UVscale->x;
			uvBuffer[index+1] = (vertice2D.y + UVtranslation->y) * UVscale->y;	
		}
		*count = uvCount;
	}
	else{
		uvBuffer = new float[0];
		*count = 0;		
	}
	return uvBuffer;
}

float* createAndFillBuffer(int largo, FbxLayerElementArrayTemplate<FbxVector4> *vector){
	float* buffer = new float[3*largo];
	for (int i=0;i<largo;i++){
		int index = 3*i;		
		glm::vec3 vec3Tangent = toVector(vector->GetAt(i));
        buffer[index] = vec3Tangent.x;
		buffer[index+1] = vec3Tangent.y;
		buffer[index+2] = vec3Tangent.z;
	}
	return buffer;
}

//Consigo el vertexBuffer
float* createAndFillVertexBuffer(FbxMesh* fbxMesh, MeshDataTransfer* meshDataTransfer, int* size){	
	vector<ControlPoint*>* controlPointsData = meshDataTransfer->controlPoints;
	controlPointsData->clear();
	FbxVector4 *controlPoints = fbxMesh->GetControlPoints();	
	int controlPointsCount = fbxMesh->GetControlPointsCount();

	*size = 3 * controlPointsCount;
	float* vertexBuffer = new float[*size];

	// Me guardo cada vertice o punto de control, pero multplicandolo por la matriz de transformacion
	for (int i=0;i<controlPointsCount;i++){
		int index = 3*i;
		glm::vec3 vertice = vec3(meshDataTransfer->transformationMatrix * vec4(toVector(controlPoints[i]), 1.0f));		
		controlPointsData->push_back(new ControlPoint(vertice));
		vertexBuffer[index] = vertice.x;
		vertexBuffer[index+1] = vertice.y;
		vertexBuffer[index+2] = vertice.z;		
	}	
	return vertexBuffer;
}

//Consigo el NormalBuffer
float* createAndFillNormalBuffer(FbxMesh* fbxMesh, int* size){		
	FbxLayer *normalLayer = fbxMesh->GetLayer(0, FbxLayerElement::eNormal);
    FbxLayerElementNormal *normalElement = normalLayer->GetNormals();
    FbxLayerElementArrayTemplate<FbxVector4> *normalArray = &(normalElement->GetDirectArray());                
    int normalCount = normalArray->GetCount();

	*size = 3*normalCount; 
	return createAndFillBuffer(normalCount, normalArray);	
}

//Consigo el indexBuffer
unsigned int* createAndFillIndexBuffer(FbxMesh* fbxMesh, int *size){		
	int poligonCount = fbxMesh->GetPolygonCount();
	//Guardo los indices
	vector<unsigned int> indices;
	for (int i=0;i < poligonCount;i++){
		for (int j=0;j < fbxMesh->GetPolygonSize(i); j++){
			if (j>2){
				// if polygon size > 2 then add first and last index
                // this triangulates the mesh
                unsigned int first = fbxMesh->GetPolygonVertex(i,0);
                unsigned int last = fbxMesh->GetPolygonVertex(i,j-1);
                indices.push_back(first);
                indices.push_back(last);
			}
            int polygonIndex = fbxMesh->GetPolygonVertex(i,j);
            indices.push_back(polygonIndex);                      
	  }	  
	}
	*size = indices.size();
	unsigned int* indexBuffer = new unsigned int[*size];
   std::vector<unsigned int>::iterator index_it;
	int v = 0;
    for (index_it = indices.begin(); index_it != indices.end(); index_it++)
    {
        indexBuffer[v] = *index_it;
        v++;
    }
	return indexBuffer;	
}

//Consigo el TangentBuffer
float* createAndFillTangentBuffer(FbxMesh* fbxMesh, int* size){	
	FbxLayer *tangentLayer = fbxMesh->GetLayer(0, FbxLayerElement::eTangent);
	FbxLayerElementArrayTemplate<FbxVector4> *tangentArray;
	FbxLayerElementTangent *tangentElement = tangentLayer->GetTangents();
	tangentArray = &(tangentElement->GetDirectArray());                
	int tangentCount = tangentArray->GetCount();

	*size = 3* tangentCount;
	return createAndFillBuffer(tangentCount, tangentArray);
}

//Consigo el binormalBuffer
float* createAndFillBinoramalBuffer(FbxMesh* fbxMesh, int *size){		

	FbxLayer *binormalLayer = fbxMesh->GetLayer(0, FbxLayerElement::eBiNormal);
	FbxLayerElementBinormal *binormalElement = binormalLayer->GetBinormals();
	FbxLayerElementArrayTemplate<FbxVector4> *binormalArray = &(binormalElement->GetDirectArray());                
	int binormalCount = binormalArray->GetCount();

	*size = 3*binormalCount;
	return createAndFillBuffer(binormalCount, binormalArray);
}


MeshDataTransfer* FbxModelLoader::processMesh(FbxNode* pNode, SkeletonDataTransfer* skeletonDataTransfer){
	//Creo el mesh hijo
	MeshDataTransfer* meshDataTransfer = new MeshDataTransfer();
	//Guardo el nombre
	meshDataTransfer->name = pNode->GetName();

	//Consigo las transformaciones locales del mesh para modificar los vertices, pero luego guardo la matriz identidad en el smf	
	meshDataTransfer->scale = toVector(pNode->LclScaling.Get());
	meshDataTransfer->rotation = toVector(pNode->LclRotation.Get());
	meshDataTransfer->position = toVector(pNode->LclTranslation.Get());
	meshDataTransfer->generateTransformationMatrix();
	
	meshDataTransfer->scale = vec3(1.0f);
	meshDataTransfer->rotation = vec3(0.0f);
	meshDataTransfer->position = vec3(0.0f);	
	//***********************************************************************************************************
	
	//Consigo el FbxMesh que tiene los datos del mesh
	FbxMesh* fbxMesh = pNode->GetMesh();
	
	//Guardo el contenido de la textura y consigo los parametros de UV´s
	glm::vec2 UVtranslation, UVscale;
	string textureMapPath = processTexture(pNode, &UVtranslation, &UVscale);
	if(!textureMapPath.empty()){
		meshDataTransfer->difuseMapFilename = textureMapPath;
	}
	//Seteo el normal map si es que hay
	textureMapPath = processNormalMap(pNode);
	if(!textureMapPath.empty()){
		meshDataTransfer->normalMapFilename = textureMapPath;
	}
	//Agrego el displacement map si es que hay
	textureMapPath = processDisplacementMap(pNode);
	if(!textureMapPath.empty()){
		meshDataTransfer->displacementMapFilename = textureMapPath;
	}
	//Proceso los UV´s
	int uvCount;
	float* uvBuffer = processUV(fbxMesh, &uvCount, &UVtranslation, &UVscale);
	meshDataTransfer->uvScale = UVscale;
	meshDataTransfer->uvTransform = UVtranslation;

	//VertexBuffer
	int size = 0;	
	float* buffer = createAndFillVertexBuffer(fbxMesh, meshDataTransfer, &size);
	meshDataTransfer->vertexBufferData = buffer;
	meshDataTransfer->vertexBufferSize = size;
	//NormalBuffer
	buffer = createAndFillNormalBuffer(fbxMesh, &size);
	meshDataTransfer->normalBufferData = buffer;
	meshDataTransfer->normalBufferSize = size;
	//TangentBuffer
	buffer = createAndFillTangentBuffer(fbxMesh, &size);
	meshDataTransfer->tangentBufferData = buffer;
	meshDataTransfer->tangentBufferSize = size;
	//BinormalBuffer
	buffer = createAndFillBinoramalBuffer(fbxMesh, &size);
	meshDataTransfer->binormalBufferData = buffer;
	meshDataTransfer->binormalBufferSize = size;
	//IndexBuffer
	unsigned int* indexBuffer = createAndFillIndexBuffer(fbxMesh, &size);
	meshDataTransfer->indexBufferData = indexBuffer;
	meshDataTransfer->indexBufferSize = size;
	//UV´s
	int texCoordsCount = 2 * uvCount;		
	meshDataTransfer->textureCoordsData = uvBuffer;
	meshDataTransfer->textureCoordsSize = texCoordsCount;

	//Asigna todos los factores (diffuse, ambient, displacement, normal, etc)
	loadFactors(meshDataTransfer, pNode);

	// Consigo, si los hay, los vinculos y pesos de cada vertice del mesh con las juntas del skeleton
	if(skeletonDataTransfer->getChilds()->size() > 0){
		processLinks(pNode, meshDataTransfer, skeletonDataTransfer);
	}
	return meshDataTransfer;
}

SkeletonDataTransfer* FbxModelLoader::processSkeleton(FbxNode* pNode, SkeletonDataTransfer* parentSkeletonDataTransfer)
{ 
	SkeletonDataTransfer* skeletonDataTransfer = new SkeletonDataTransfer();
    FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();

	// Nombre del Joint (unico)
	skeletonDataTransfer->jointName = pNode->GetName();
    
	return skeletonDataTransfer;
}


void adjustJointsRotation(vec3& rotation, FbxAMatrix& mGlobalTransform ){	
	//vec3 globalRotation = toVector(mGlobalTransform.GetR());
	//if(globalRotation.x < 0.0f && globalRotation.z >= 0.0f){
		//rotation.z = -rotation.z;
	//}
}

void FbxModelLoader::processAnimations(SkeletonDataTransfer* skeletonJoint, FbxNode* pNode, FbxCluster* lCluster){
	// Now only supports one take
	FbxAnimStack* currAnimStack = this->lScene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = currAnimStack->GetName();
	string mAnimationName = string(animStackName.Buffer());
	FbxTakeInfo* takeInfo = this->lScene->GetTakeInfo(animStackName);
	FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	unsigned int mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
	
	for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
	{
		FbxTime currTime;
		currTime.SetFrame(i, FbxTime::eFrames24);			
		FbxAMatrix mGlobalTransform = lCluster->GetLink()->EvaluateGlobalTransform(currTime);
		FbxAMatrix mLocalTransform = lCluster->GetLink()->EvaluateLocalTransform(currTime);

		// Consigo la transformacion, rotacion y escala
		vec3 translation = toVector(mGlobalTransform.GetT());
		vec3 rotation = toVector(mLocalTransform.GetR());
		vec3 scale = toVector(mGlobalTransform.GetS());

		// REVIEW
		//adjustJointsRotation(rotation, mGlobalTransform);
									
		// Finalemente me guardo cada componente de la transformacion en un nuevo keyframe
		KeyFrameBruteDataTransfer* keyframe = new KeyFrameBruteDataTransfer();	
		keyframe->position = translation;
		keyframe->rotation = rotation;
		keyframe->scale = scale;		

		// Agrego el keyframe al joint original
		skeletonJoint->addKeyFrame(keyframe);
	}
}

void FbxModelLoader::processLinks(FbxNode* pNode, MeshDataTransfer* meshDataTransfer, SkeletonDataTransfer* skeletonDataTransfer)
{  
    int lSkinCount=0;
    int lClusterCount=0;
    FbxCluster* lCluster;
	meshDataTransfer->hasSkiningInfo = true;
	
	FbxGeometry* pGeometry = pNode->GetMesh();
    lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);
	vector<ControlPoint*>* controlPointsData = meshDataTransfer->controlPoints;

    //lLinkCount = pGeometry->GetLinkCount();
    for(int i=0; i != lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        for (int j = 0; j != lClusterCount; ++j)
        {
            lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j); 
			SkeletonDataTransfer* originalSkelletonJoint = ImportUtilities::findJointSkelletonUsingName(lCluster->GetLink()->GetName(), skeletonDataTransfer);
			          
			// Asocio cada junta con el vertice que le corresponde			
			unsigned int numOfIndices = lCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				ControlPointSkinJointData* jointData = new ControlPointSkinJointData();
				originalSkelletonJoint->setBindedToMesh(true);
				jointData->skeletonJointIndex = originalSkelletonJoint;
				jointData->jointToVertexWeight = lCluster->GetControlPointWeights()[i];
				controlPointsData->at(lCluster->GetControlPointIndices()[i])->skinInfo->push_back(jointData);
			}	

			// Actualizo las transformaciones de los Joints en el esqueleto
			FbxAMatrix globalBindposeMatrix;
            globalBindposeMatrix = lCluster->GetTransformMatrix(globalBindposeMatrix);
			globalBindposeMatrix = lCluster->GetTransformLinkMatrix(globalBindposeMatrix);
			if (lCluster->GetAssociateModel() != NULL)
            	globalBindposeMatrix = lCluster->GetTransformAssociateModelMatrix(globalBindposeMatrix);

			FbxAMatrix mLocalTransform = lCluster->GetLink()->EvaluateLocalTransform();

			// Consigo la transformacion, rotacion y escala
			vec3 translation = toVector(globalBindposeMatrix.GetT());
			vec3 rotation = toVector(mLocalTransform.GetR());
			vec3 scale = toVector(globalBindposeMatrix.GetS());
									
			// Finalemente me guardo cada componente de la transformacion			
			originalSkelletonJoint->position = translation;
			originalSkelletonJoint->rotation = rotation;
			originalSkelletonJoint->scale = scale;	

			// REVIEW: En los bipeds se invierte la rotacion en los huesos "negativos"
			//adjustJointsRotation(originalSkelletonJoint->rotation, globalBindposeMatrix);

			// Si hay animaciones las proceso
			processAnimations(originalSkelletonJoint, pNode, lCluster);
        }
    }
	// Fuerzo a que todos los puntos de control tengan 4 skinInfo
	for(unsigned int i=0; i < controlPointsData->size(); i++){
		ControlPoint* controlPoint = controlPointsData->at(i);
		while(controlPoint->skinInfo->size() < 4){
			controlPoint->skinInfo->push_back(new ControlPointSkinJointData());
		}
	}
}

bool FbxModelLoader::filterUnbindedSkeletonJoints(SkeletonDataTransfer* skeletonJoint, SkeletonDataTransfer* jointParent)
{
	// Si no tiene la marca de binded con un mesh, lo filtro y asigno a su padre como padre de sus hijos
	if(!skeletonJoint->isBindedToMesh() && jointParent != NULL){

		// El padre del joint es ahora el padre de los hijos		
		list<SkeletonDataTransfer*>::iterator it = skeletonJoint->getChilds()->begin();
		while(it != skeletonJoint->getChilds()->end()){
			jointParent->addChild(*it);
			++it;
		}
		// Borro el joint y termine
		delete skeletonJoint;
		return true;
	}
	else{
		// El joint esta bindeado con el mesh, me guardo su index
		skeletonJoint->jointIndex = this->jointIndexCount;
		this->jointIndexCount++;	

		// Proceso los hijos
		list<SkeletonDataTransfer*>::iterator it = skeletonJoint->getChilds()->begin();
		while(it != skeletonJoint->getChilds()->end()){
			// Si se borro al hijo, lo borro de mis childs
			if(this->filterUnbindedSkeletonJoints(*it, skeletonJoint))
				it = skeletonJoint->getChilds()->erase(it);
			else
				++it;
		}
	}
	return false;
}

