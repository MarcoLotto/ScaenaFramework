/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Scene.h"
#include "XmlTree.h"

class SceneWriter{

private:
	static SceneWriter* instance;
	void parseObjets(XmlTreeNode* node, ObjectManager* objectManager);
	void parseLights(XmlTreeNode* node, LightingManager* lightingManager);
	void serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent);
	void serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent);
	void serializeTransformationsToNode(Object* object, XmlTreeNode* parentNode);

	void serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent);
	void serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent);
	void serializeToNode(string propertyName, float vector, XmlTreeNode* parent);

	void serializePointLight(SceneLight* light, XmlTreeNode* parent);	
	void serializeDirectionalLight(SceneLight* light, XmlTreeNode* parent);	
	void serializeSpotLight(SceneLight* light, XmlTreeNode* parent);	
	void serializePointDifuseLight(SceneLight* light, XmlTreeNode* parent);			
	void serializeShadow(SceneLight* light, XmlTreeNode* lightNode);

public:
	static SceneWriter* getInstance();

	XmlTree* sceneToXmlTree(Scene* scene);
	void writeSceneToFileSystem(string filename, Scene* scene);
};