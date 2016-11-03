/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ScaenaModelLoaderHelper.h"
#include "XmlUtils.h"
#include "MeshLoadException.h"
#include <string.h>
#include "SkinnedMesh.h"
#include "MeshSkeletonJoint.h"


MeshSkeleton* ScaenaModelLoaderHelper::processSkeleton(XmlTreeNode* skeletonNode){
	//Creo el skeleton
	MeshSkeleton* skeleton = new MeshSkeleton();

	//Guardo el nombre
	XmlNodeAttribute* attribute = skeletonNode->searchForAttribute("Name");
	if(attribute == NULL){
		throw new MeshLoadException("Falta el atributo Name en la declaracion de un skeleton");
	}	
 	skeleton->setName(attribute->getValue());

	// Proceso cada uno de los joints hijos
	unsigned int iteration = 0;
	XmlTreeNode* childJointNode = skeletonNode->searchDirectChild("Joint", 0);
	while(childJointNode != NULL){
		processSkeletonJoint(childJointNode, skeleton);
		iteration++;
		childJointNode = skeletonNode->searchDirectChild("Joint", iteration);
	}
	return skeleton;
}

void ScaenaModelLoaderHelper::processSkeletonJoint(XmlTreeNode* skeletonNode, MeshSkeleton* parentSkeleton){
	//Creo el skeleton hijo
	MeshSkeletonJoint* joint = new MeshSkeletonJoint(parentSkeleton);

	//Guardo el nombre
	XmlNodeAttribute* attribute = skeletonNode->searchForAttribute("Name");
	if(attribute == NULL){
		throw new MeshLoadException("Falta el atributo Name en la declaracion de un skeleton joint");
	}	
 	joint->setName(attribute->getValue());

 	//Guardo el index
	attribute = skeletonNode->searchForAttribute("Index");
	if(attribute == NULL){
		throw new MeshLoadException("Falta el atributo Index en la declaracion de un skeleton joint");
	}	
 	joint->setIndex(atoi(attribute->getValue().c_str()));

	//Consigo las transformaciones del joint al momento de bindearlo con el mesh
	processMeshTransformations(joint->getName() + "(Skeleton)", "TODO", joint->getGlobalBindposeTransformation(), skeletonNode);
	
	// Agrego el joint hijo al padre
	parentSkeleton->addChild(joint);

	// Proceso cada uno de los hijos
	unsigned int iteration = 0;
	XmlTreeNode* childJointNode = skeletonNode->searchDirectChild("Joint", 0);
	while(childJointNode != NULL){
		processSkeletonJoint(childJointNode, joint);
		iteration++;
		childJointNode = skeletonNode->searchDirectChild("Joint", iteration);
	}
}

void ScaenaModelLoaderHelper::processMeshTransformations(string meshName, string meshFilePath, Transformation* transformation, XmlTreeNode* meshNode){
	XmlTreeNode* tranformationsNode = meshNode->searchDirectChild("Transformations", 0);
	if(tranformationsNode == NULL){
		throw new MeshLoadException("Falta el nodo Transformations en el mesh: " + meshName + " dentro del modelo: " + meshFilePath);
	}
	XmlTreeNode* node = tranformationsNode->searchDirectChild("Translation", 0);
	if(node == NULL){
		throw new MeshLoadException("Falta el nodo Translation en el mesh: " + meshName + " dentro del modelo: " + meshFilePath);
	}
	transformation->setPosition(XmlUtils::getVec3FromNode(node));

	node = tranformationsNode->searchDirectChild("Rotation", 0);
	if(node == NULL){
		throw new MeshLoadException("Falta el nodo Rotation en el mesh: " + meshName + " dentro del modelo: " + meshFilePath);
	}
	transformation->setRotation(XmlUtils::getVec3FromNode(node));

	node = tranformationsNode->searchDirectChild("Scale", 0);
	if(node == NULL){
		throw new MeshLoadException("Falta el nodo Scale en el mesh: " + meshName + " dentro del modelo: " + meshFilePath);
	}
	transformation->setScale(XmlUtils::getVec3FromNode(node));
}