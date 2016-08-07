/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlTree.h"
#include "Object.h"

class ObjectLoader{

private:
	static void serializeTransformationsToNode(Mesh* mesh, XmlTreeNode* parentNode);
	static void processMesh(Mesh* mesh, XmlTreeNode* node);
	static void loadMesh(Mesh* mesh, XmlTreeNode* node);
	static XmlTreeNode* writeMeshToNode(Mesh* mesh, XmlTreeNode* parent);
	static void writeFullMeshToNodes(XmlTreeNode* node, Mesh* mesh);
	static vec3 getVec3FromNode(XmlTreeNode* node);
	static vec2 getVec2FromNode2(XmlTreeNode* node);
	static vec3 getVec3LightParam(string paramName, XmlTreeNode* lightNode);
	static vec2 getVec2ShadowParam(string paramName, XmlTreeNode* lightNode);
	static float getFloatLightParam(string paramName, XmlTreeNode* lightNode);
	static float getFloatParam(string paramName, XmlTreeNode* node);
	static void serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent);
	static void serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent);

	static void serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent);
	static void serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent);
	static void serializeToNode(string propertyName, float vector, XmlTreeNode* parent);

	static void loadLights(Object* object, XmlTreeNode* objectNode);
	static ObjectLight* processDirectionalLight(Object* object, XmlTreeNode* lightNode);
	static ObjectLight* processPointDiffuseLight(Object* object, XmlTreeNode* lightNode);
	static ObjectLight* processPointLight(Object* object, XmlTreeNode* lightNode);
	static ObjectLight* processSpotLight(Object* object, XmlTreeNode* lightNode);
	static void processShadow(XmlTreeNode* lightNode, ObjectLight* light);
	
	static void writeAllLightsToNodes(XmlTreeNode* objectNode, vector<ObjectLight*>* objectLights);
	static void writeLightToNode(ObjectLight* light, XmlTreeNode* node);
	static void serializePointLight(SceneLight* light, XmlTreeNode* parent);	
	static void serializeDirectionalLight(SceneLight* light, XmlTreeNode* parent);	
	static void serializeSpotLight(SceneLight* light, XmlTreeNode* parent);	
	static void serializePointDifuseLight(SceneLight* light, XmlTreeNode* parent);			
	static void serializeShadow(SceneLight* light, XmlTreeNode* lightNode);

public:
	static XmlTree* objectToXlmTree(Object* object);
	static Object* loadObjectFromFile(const char* filename);
};