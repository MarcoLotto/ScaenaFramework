/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ScaenaModelWriter.h"
#include "XmlParser.h"
#include "XmlUtils.h"
#include "StringUtils.h"
#include "Logger.h"

void ScaenaModelWriter::writeModel(string filename, ModelDataTransfer* modelDataTransfer){
	Logger::getInstance()->logInfo(new Log("Procesando el SMF de salida..."));
	XmlTree* xmlTree = modelToXmlTree(modelDataTransfer);
	XmlParser::writeXmlToFile(getFinalFilename(filename).c_str(), xmlTree);
}

string ScaenaModelWriter::getFinalFilename(string originalFilename){
	return originalFilename.erase(originalFilename.length()-3, originalFilename.length()) + "smf";
}

XmlTree* ScaenaModelWriter::modelToXmlTree(ModelDataTransfer* modelDataTransfer){
	XmlTree* xmlTree = new XmlTree();
	XmlTreeNode* rootNode = xmlTree->getRootNode();

	// Agrego el tag inicial de modelo
	XmlTreeNode* modelNode = new XmlTreeNode("Model", rootNode);
	rootNode->addChild(modelNode);

	// Mando a agregar al arbol XML a todos los componentes del modelo
	meshToXmlTree(modelNode, modelDataTransfer);
	skeletonToXmlTree(modelNode, modelDataTransfer);
	
	return xmlTree;
}

void ScaenaModelWriter::meshToXmlTree(XmlTreeNode* parentNode, ModelDataTransfer* modelDataTransfer){	
	// Consigo el nodo root de mesh
	MeshDataTransfer* rootMeshNode = modelDataTransfer->getRootMesh();

	if(rootMeshNode != NULL){
		// Recorro todos los hijos y proceso cada uno
		list<MeshDataTransfer*>::iterator it = rootMeshNode->getChilds()->begin();
		while(it != rootMeshNode->getChilds()->end()){
			processMesh(parentNode, *it);
			++it;
		}
	}
	else{
		Logger::getInstance()->logInfo(new Log("Warning: El archivo no posee informacion de mesh"));
	}
}

void ScaenaModelWriter::skeletonToXmlTree(XmlTreeNode* parentNode, ModelDataTransfer* modelDataTransfer){
	// Consigo el nodo root del esqueleto
	SkeletonDataTransfer* rootSkeletonNode = modelDataTransfer->getRootSkeleton();

	if(rootSkeletonNode != NULL){
		Logger::getInstance()->logInfo(new Log("El archivo posee informacion de skeleton"));

		// Recorro todos los hijos y proceso cada uno
		list<SkeletonDataTransfer*>::iterator it = rootSkeletonNode->getChilds()->begin();
		while(it != rootSkeletonNode->getChilds()->end()){
			processSkeleton(parentNode, *it);
			++it;
		}
	}
}

unsigned int unsignedIntDecoder(unsigned char* packed){	
	return ((((((packed[0] << 8) | packed[1]) << 8) | packed[2]) << 8) | packed[3]);
}

float floatDecoder(unsigned char* packed){
	unsigned int unpackedValue = unsignedIntDecoder(packed);
	float decodedValue = ((unpackedValue>>16)&0x7fff); // whole part
    decodedValue += (unpackedValue&0xffff) / 65536.0f; // fraction
    if (((unpackedValue>>31)&0x1) == 0x1) { decodedValue = -decodedValue; }  // sign bit set   
    return decodedValue;
}


void ScaenaModelWriter::processMesh(XmlTreeNode* parentNode, MeshDataTransfer* meshDataTransfer){
	// Creo un nuevo nodo para el mesh
	XmlTreeNode* meshNode = new XmlTreeNode("Mesh", parentNode);
	parentNode->addChild(meshNode);
	meshNode->addAttribute(new XmlNodeAttribute("Name", meshDataTransfer->name));

	// Seteo sus transformaciones
	serializeTransformationsToNode(meshNode, meshDataTransfer);

	//Me guardo los buffers
	XmlUtils::parseBufferInBinaryToNode("VertexBuffer", meshDataTransfer->vertexBufferData, meshDataTransfer->vertexBufferSize, meshNode);
	XmlUtils::parseBufferInBinaryToNode("NormalBuffer", meshDataTransfer->normalBufferData, meshDataTransfer->normalBufferSize, meshNode);
	XmlUtils::parseBufferInBinaryToNode("TangentBuffer", meshDataTransfer->tangentBufferData, meshDataTransfer->tangentBufferSize, meshNode);
	XmlUtils::parseBufferInBinaryToNode("BinormalBuffer", meshDataTransfer->binormalBufferData, meshDataTransfer->binormalBufferSize, meshNode);
	XmlUtils::parseBufferInBinaryToNode("IndexBuffer", meshDataTransfer->indexBufferData, meshDataTransfer->indexBufferSize, meshNode);
	XmlUtils::parseBufferInBinaryToNode("TextureCoordsBuffer", meshDataTransfer->textureCoordsData, meshDataTransfer->textureCoordsSize, meshNode);

	// Me guardo la informacion de skinning de este mesh
	if(meshDataTransfer->hasSkiningInfo)
		serializeSkiningInfoToNode(meshNode, meshDataTransfer);

	// Ahora guardo la informacion de texturas
	serializeTexturesToNode(meshNode, meshDataTransfer);

	// Recorro todos los hijos y proceso cada uno
	list<MeshDataTransfer*>::iterator it = meshDataTransfer->getChilds()->begin();
	while(it != meshDataTransfer->getChilds()->end()){
		processMesh(meshNode, *it);
		++it;
	}	
}

void ScaenaModelWriter::processSkeleton(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer){
	// Creo un nuevo nodo para el mesh
	XmlTreeNode* skeletonNode = new XmlTreeNode("Skeleton", parentNode);
	parentNode->addChild(skeletonNode);
	skeletonNode->addAttribute(new XmlNodeAttribute("Name", skeletonDataTransfer->jointName));	
	
	// Recorro todos los hijos y proceso cada uno
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){
		processJoint(skeletonNode, *it);
		++it;
	}	
}

void ScaenaModelWriter::processJoint(XmlTreeNode* parentNode, SkeletonDataTransfer* skeletonDataTransfer){
	// Creo un nuevo nodo para el mesh
	XmlTreeNode* jointNode = new XmlTreeNode("Joint", parentNode);
	parentNode->addChild(jointNode);
	jointNode->addAttribute(new XmlNodeAttribute("Name", skeletonDataTransfer->jointName));
	jointNode->addAttribute(new XmlNodeAttribute("Index", StringUtils::toString(skeletonDataTransfer->jointIndex - 2)));  // -2 es porque no se cuenta ni el dt root ni el skeleton root
	
	// Me guardo las transformaciones (globales) del joint a la hora de cuando fue bindeado con el mesh
	serializeTransformationsToNode(jointNode, skeletonDataTransfer);

	// Recorro todos los hijos y proceso cada uno
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){
		processJoint(jointNode, *it);
		++it;
	}	
}

vec3 getNormalizedValue(vec3 value){
	if(value.x > 0.0f && value.x < 0.00001f){
		value.x = 0.0f;
	}
	if(value.y > 0.0f && value.y < 0.00001f){
		value.y = 0.0f;
	}
	if(value.z > 0.0f && value.z < 0.00001f){
		value.z = 0.0f;
	}
	return value;
}

void ScaenaModelWriter::serializeTransformationsToNode(XmlTreeNode* parentNode, InformationDataTransfer* dataTransfer){
	XmlTreeNode* transformationNode = new XmlTreeNode("Transformations", parentNode);
	parentNode->addChild(transformationNode);
	// Las propiedades hijas		
	XmlUtils::serializeToNode("Translation", getNormalizedValue(dataTransfer->position), transformationNode);
	XmlUtils::serializeToNode("Rotation", getNormalizedValue(dataTransfer->rotation), transformationNode);
	XmlUtils::serializeToNode("Scale", getNormalizedValue(dataTransfer->scale), transformationNode);
}

void ScaenaModelWriter::serializeTexturesToNode(XmlTreeNode* parentNode, MeshDataTransfer* meshDataTransfer){
	XmlTreeNode* texturesNode = new XmlTreeNode("Textures", parentNode);
	parentNode->addChild(texturesNode);
	// Agrego las transformaciones UV
	XmlUtils::serializeToNode("UVTranslation", meshDataTransfer->uvTransform, texturesNode);
	XmlUtils::serializeToNode("UVScale", meshDataTransfer->uvScale, texturesNode);

	// Agrego las rutas de las texturas
	if(!meshDataTransfer->difuseMapFilename.empty())
		XmlUtils::serializeToNode("Difuse", meshDataTransfer->difuseMapFilename, texturesNode);
	if(!meshDataTransfer->normalMapFilename.empty())
		XmlUtils::serializeToNode("Normal", meshDataTransfer->normalMapFilename, texturesNode);
	if(!meshDataTransfer->displacementMapFilename.empty())
		XmlUtils::serializeToNode("Displacement", meshDataTransfer->displacementMapFilename, texturesNode);

	// Agrego los factores
	if(!meshDataTransfer->displacementMapFilename.empty())
		XmlUtils::serializeToNode("DisplacementFactor", meshDataTransfer->displacementFactor, texturesNode);
}

unsigned int getNullSafeJointIndexValue(SkeletonDataTransfer* pointedSkeletonJoint){
	if(pointedSkeletonJoint != NULL && pointedSkeletonJoint->jointIndex >= 2){
		// El -2 es porque se ignoran el data transfer root y el root de skeleton
		return pointedSkeletonJoint->jointIndex - 2;
	}
	return 0;
}

void ScaenaModelWriter::serializeSkiningInfoToNode(XmlTreeNode* parentNode, MeshDataTransfer* meshDataTransfer){
	// Creo un nuevo nodo para el skining
	XmlTreeNode* skinNode = new XmlTreeNode("SkinInfo", parentNode);
	parentNode->addChild(skinNode);

	vector<ControlPoint*>* controlPoints = meshDataTransfer->controlPoints;

	//jointNode->addAttribute(new XmlNodeAttribute("Name", skeletonDataTransfer->jointName));
	//jointNode->addAttribute(new XmlNodeAttribute("Index", StringUtils::toString(skeletonDataTransfer->jointIndex)));
	
	// Genero el buffer de JointIndex a los que apuntarán cada vertice del mesh
	unsigned int* jointIndexes = new unsigned int[controlPoints->size() * 4];  // El *4 es porque hay 4 joints por vertice	
	for(unsigned int i=0; i < controlPoints->size(); i++){
		vector<ControlPointSkinJointData*>* skinInfo = controlPoints->at(i)->skinInfo;
		jointIndexes[i*4] = getNullSafeJointIndexValue(skinInfo->at(0)->skeletonJointIndex);
		jointIndexes[i*4+1] = getNullSafeJointIndexValue(skinInfo->at(1)->skeletonJointIndex);
		jointIndexes[i*4+2] = getNullSafeJointIndexValue(skinInfo->at(2)->skeletonJointIndex);
		jointIndexes[i*4+3] = getNullSafeJointIndexValue(skinInfo->at(3)->skeletonJointIndex);		
	}
	XmlUtils::parseBufferInBinaryToNode("JointIndexes", jointIndexes, controlPoints->size() * 4, skinNode);	
	delete jointIndexes;

	// Genero el buffer de JointWeights que contienen que tanto pesa un joint a la hora de deformar el vertice
	float* jointWeights = new float[controlPoints->size() * 4];  // El *4 es porque hay 4 joint weights por vertice	
	for(unsigned int i=0; i < controlPoints->size(); i++){
		vector<ControlPointSkinJointData*>* skinInfo = controlPoints->at(i)->skinInfo;
		jointWeights[i*4] = skinInfo->at(0)->jointToVertexWeight;
		jointWeights[i*4+1] = skinInfo->at(1)->jointToVertexWeight;
		jointWeights[i*4+2] = skinInfo->at(2)->jointToVertexWeight;
		jointWeights[i*4+3] = skinInfo->at(3)->jointToVertexWeight;
		
	}
	XmlUtils::parseBufferInBinaryToNode("JointWeights", jointWeights, controlPoints->size() * 4, skinNode);
	delete jointWeights;
}

