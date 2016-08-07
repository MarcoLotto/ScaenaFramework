/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "Mesh.h"
#include "ScaenaModelLoader.h"
#include "FileTexture.h"
#include "ShaderManager.h"
#include "MemoryManager.h"
#include "XmlUtils.h"
#include "MeshLoadException.h"
#include <string.h>
#include "WindowConnector.h"
#include "SkinnedMesh.h"
#include "MeshSkeletonJoint.h"
#include "ScaenaModelLoaderHelper.h"
#include "ScaenaAnimationLoader.h"

#define DIFFUSE_TEXTURES_PATH WindowConnector::getBaseApplicationPath() + "Textures/Difusas/"
#define NORMAL_TEXTURES_PATH WindowConnector::getBaseApplicationPath() + "Textures/NormalMaps/"
#define DISPLACEMENT_TEXTURES_PATH WindowConnector::getBaseApplicationPath() + "Textures/DisplacementMaps/"

ScaenaModelLoader::ScaenaModelLoader(){
}

ScaenaModelLoader::~ScaenaModelLoader(){
}

void validateFile(const char* filename){
	int size = strlen(filename);
	if(filename[size-3]=='s' && filename[size-2]=='m' && filename[size-1]=='f')
		return;
	if(filename[size-3]=='S' && filename[size-2]=='M' && filename[size-1]=='F')
		return;
	throw new MeshLoadException("El archivo " + string(filename) + " no es un .smf valido");
}

Mesh* ScaenaModelLoader::getRequiredMeshInstance(MeshSkeleton* skeleton){
	if(skeleton != NULL){
		return new SkinnedMesh(skeleton);
	}	
	return new Mesh();	
}

Mesh* ScaenaModelLoader::loadModel(const char* filename){
	// Valido que el tipo de archivo sea correcto
	validateFile(filename);

	Mesh* mesh = NULL;
	MeshSkeleton* skeleton = NULL;
	//Leo el archivo y lo parseo en un arbol
	XmlTree* xmlTree = XmlParser::parse(string(WindowConnector::getBaseApplicationPath() + filename).c_str());
	//***Transformo el arbol en un Mesh***
	XmlTreeNode* root =  xmlTree->getRootNode();
	//Primiero busco la etiqueta de Model
	XmlTreeNode* modelNode = NULL;
	for(int i=0; i < root->getChildNodes().size(); i++){
		modelNode = root->getChildNodes()[i];
		if(modelNode->getName().compare("Model")==0){

			// Busco si hay algun skeleton, de ser asi lo cargo y luego instancio un SkinnedMesh
			XmlTreeNode* skeletonNode = modelNode->searchDirectChild("Skeleton", 0);
			if(skeletonNode != NULL){
				skeleton = ScaenaModelLoaderHelper::processSkeleton(skeletonNode);	

				// Mando a cargar archivos de animacion (puede no haber)
				ScaenaAnimationLoader::loadAnimations(filename, skeleton);

			}			
			// Instancio la instancia correcta de mesh para este caso
			mesh = getRequiredMeshInstance(skeleton);					
			break;
		}
	}
    if(mesh == NULL){
        if(xmlTree != NULL){ delete xmlTree; }
        return NULL; //Error
    }

	// Le seteo las propiedades al mesh root
	mesh->setFilePath(filename);
	mesh->setName("root");

	// Proceso cada uno de los hijos
	unsigned int iteration = 0;
	XmlTreeNode* childMeshNode = modelNode->searchDirectChild("Mesh", 0);
	while(childMeshNode != NULL){
		processMesh(mesh, childMeshNode, skeleton);
		iteration++;
		childMeshNode = modelNode->searchDirectChild("Mesh", iteration);
	}
    delete xmlTree;
	return mesh;
}


void ScaenaModelLoader::processMesh(Mesh* parentMesh, XmlTreeNode* meshNode, MeshSkeleton* skeleton){
	//Creo el mesh hijo
	Mesh* mesh = getRequiredMeshInstance(skeleton);	
	mesh->setFilePath(parentMesh->getFilePath());

	//Guardo el nombre
	XmlNodeAttribute* attribute = meshNode->searchForAttribute("Name");
	if(attribute == NULL){
		throw new MeshLoadException("Falta el atributo Name en la declaracion de un mesh en: " + parentMesh->getFilePath());
	}	
 	mesh->setName(attribute->getValue());

	// Consigo las transformaciones del mesh
	ScaenaModelLoaderHelper::processMeshTransformations(mesh->getName(), mesh->getFilePath(), mesh->getTransformation(), meshNode);

	// Cargo la información de textura
	processMeshTextures(mesh, meshNode);

	//VertexBuffer
	int size = 0;
	MeshBuffers* meshBuffers = mesh->getMeshBuffers();
	float* buffer = XmlUtils::createFloatBufferFromBinaryNode("VertexBuffer", meshNode, &size);	
	meshBuffers->setVertexBuffer(buffer, size);
	//NormalBuffer
	buffer = XmlUtils::createFloatBufferFromBinaryNode("NormalBuffer", meshNode, &size);	
	meshBuffers->setNormalBuffer(buffer, size);
	//TangentBuffer
	buffer = XmlUtils::createFloatBufferFromBinaryNode("TangentBuffer", meshNode, &size);	
	meshBuffers->setTangentBuffer(buffer, size);
	//BinormalBuffer
	buffer = XmlUtils::createFloatBufferFromBinaryNode("BinormalBuffer", meshNode, &size);	
	meshBuffers->setBinormalBuffer(buffer, size);	
	//Coordenadas de textura
	buffer = XmlUtils::createFloatBufferFromBinaryNode("TextureCoordsBuffer", meshNode, &size);	
	meshBuffers->setTextureCoords(buffer, size);	
	//IndexBuffer
	unsigned int* indexBuffer = XmlUtils::createUnsignedIntBufferFromBinaryNode("IndexBuffer", meshNode, &size);	
	meshBuffers->setIndexBuffer(indexBuffer, size);

	// Si tengo un esqueleto, entonces me guardo los valores de los joints con el mesh
	if(skeleton != NULL){
		XmlTreeNode* skinInfoNode = meshNode->searchDirectChild("SkinInfo", 0);
		if(skinInfoNode != NULL){
			//JointIndexBuffer
			unsigned short* jointBuffer = XmlUtils::createUnsignedShortBufferFromBinaryNode("JointIndexes", skinInfoNode, &size);
			meshBuffers->setJointIndexBuffer(jointBuffer, size);
			//JointWeightBuffer
			buffer = XmlUtils::createFloatBufferFromBinaryNode("JointWeights", skinInfoNode, &size);	
			meshBuffers->setJointWeightBuffer(buffer, size);
		}
		// Me guardo quien es el root mesh
		((SkinnedMesh*) parentMesh)->asignRootSkinnedMeshToChild((SkinnedMesh*) mesh);
	}	
		
	//Asigno un shader
	mesh->setShaderProgram(asignarShader( mesh, (skeleton != NULL) ));

	// Agrego el mesh hijo al padre
	parentMesh->addChild(mesh);

	// Proceso cada uno de los hijos
	unsigned int iteration = 0;
	XmlTreeNode* childMeshNode = meshNode->searchDirectChild("Mesh", 0);
	while(childMeshNode != NULL){
		processMesh(mesh, childMeshNode, skeleton);
		iteration++;
		childMeshNode = meshNode->searchDirectChild("Mesh", iteration);
	}
}

void ScaenaModelLoader::processMeshTextures(Mesh* mesh, XmlTreeNode* meshNode){
	XmlTreeNode* tranformationsNode = meshNode->searchDirectChild("Textures", 0);
	if(tranformationsNode == NULL) return;

	// Transformaciones UV
	XmlTreeNode* node = tranformationsNode->searchDirectChild("UVTranslation", 0);
	if(node != NULL){
		mesh->getUVTransformations()->setUvTranslation(XmlUtils::getVec2FromNode(node));
	}
	node = tranformationsNode->searchDirectChild("UVScale", 0);
	if(node != NULL){
		mesh->getUVTransformations()->setUvScale(XmlUtils::getVec2FromNode(node));
	}

	// Filepath a texturas
	node = tranformationsNode->searchDirectChild("Difuse", 0);
	if(node != NULL){
		mesh->getDiffuseMap()->setMap(node->getValue());
	}
	node = tranformationsNode->searchDirectChild("Normal", 0);
	if(node != NULL){
		mesh->getNormalMap()->setMap(node->getValue());
	}
	node = tranformationsNode->searchDirectChild("Displacement", 0);
	if(node != NULL){
		mesh->getDisplacementMap()->setMap(node->getValue());
	}

	// Factores
	node = tranformationsNode->searchDirectChild("DisplacementFactor", 0);
	if(node != NULL){
		mesh->getDisplacementMap()->setDisplacementFactor(atof(node->getValue().c_str()));
	}
}

//Consigue el mejor shader disponible que se adapte con el mesh
MeshDrawerShader* ScaenaModelLoader::asignarShader(Mesh* mesh, bool hasSkeleton){
	return ShaderManager::getInstance()->evaluateTheBestShader(mesh, hasSkeleton);
}








