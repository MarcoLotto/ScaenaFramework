/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SceneWriter.h"
#include "XmlParser.h"
#include "StringUtils.h"

SceneWriter* SceneWriter::instance = NULL;

SceneWriter* SceneWriter::getInstance(){
	if(instance == NULL){
		instance = new SceneWriter();
	}
	return instance;
}

XmlTree* SceneWriter::sceneToXmlTree(Scene* scene){
	XmlTree* xmlTree = new XmlTree();
	XmlTreeNode* node = xmlTree->getRootNode();
	XmlTreeNode* sceneNode = new XmlTreeNode("Scene", node);
	node->addChild(sceneNode);
	sceneNode->createAttribute("name", scene->getName());

	// Primero guardo en el xml todos los objetos de la escena
	this->parseObjets(sceneNode, scene->getObjectManager());

	// Ahora guardo todas las luces
	this->parseLights(sceneNode, scene->getLightingManager());

	// Ahora guardo las camaras
	// TODO

	return xmlTree;
}

void SceneWriter::writeSceneToFileSystem(string filename, Scene* scene){
	XmlTree* xmlTree = sceneToXmlTree(scene);
	XmlParser::writeXmlToFile(filename.c_str(), xmlTree);
}

void SceneWriter::parseObjets(XmlTreeNode* node, ObjectManager* objectManager){
	// Recorro cada objeto de la escena y lo guardo en el tree
	for(int i=0; objectManager->getObjectByIteration(i) != NULL; i++){
		Object* object = objectManager->getObjectByIteration(i);
		
		XmlTreeNode* objectNode = new XmlTreeNode("Object", node);
		node->addChild(objectNode);
		objectNode->createAttribute("path", object->getFilename());
		this->serializeTransformationsToNode(object, objectNode);
	}
}

void SceneWriter::serializeTransformationsToNode(Object* object, XmlTreeNode* parentNode){
	XmlTreeNode* transformationNode = new XmlTreeNode("Transformations", parentNode);
	parentNode->addChild(transformationNode);
		// Las propiedades hijas
		Transformation* transformation = object->getTransformation();
		this->serializeToNode("Translation", transformation->getPosition(), transformationNode);
		this->serializeToNode("Rotation", transformation->getRotation(), transformationNode);
		this->serializeToNode("Scale", transformation->getScale(), transformationNode);
}

void SceneWriter::parseLights(XmlTreeNode* node, LightingManager* lightingManager){
	list<SceneLight*>* lights = lightingManager->getLights();
	list<SceneLight*>::iterator it = lights->begin();
	for(; it != lights->end(); it++){
		SceneLight* light = (*it);
		int lightType = light->getLightType();		
		switch(lightType){
			case LIGHT_TYPE_POINT:
				this->serializePointLight(light, node);
				break;
			case LIGHT_TYPE_DIRECCIONAL:
				this->serializeDirectionalLight(light, node);
				break;
			case LIGHT_TYPE_SPOT:
				this->serializeSpotLight(light, node);
				break;
			case LIGHT_TYPE_POINT_DIFFUSE:
				this->serializePointDifuseLight(light, node);
				break;
		};
	}
}

void SceneWriter::serializePointLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Point");
	parent->addChild(lightNode);

	this->serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	this->serializeToNode("Intensity", light->getIntensity(), lightNode);
	this->serializeShadow(light, lightNode);
}

void SceneWriter::serializeDirectionalLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Directional");
	parent->addChild(lightNode);

	this->serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	this->serializeToNode("Direction", light->getDirection(), lightNode);
	this->serializeToNode("Intensity", light->getIntensity(), lightNode);
	this->serializeShadow(light, lightNode);
}

void SceneWriter::serializeSpotLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Spot");
	parent->addChild(lightNode);

	this->serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	this->serializeToNode("Direction", light->getDirection(), lightNode);
	this->serializeToNode("Intensity", light->getIntensity(), lightNode);
	XmlTreeNode* factorNode = new XmlTreeNode("Factor", StringUtils::toString(light->getSpotFactor()), lightNode);
	lightNode->addChild(factorNode);
	this->serializeShadow(light, lightNode);
}

void SceneWriter::serializePointDifuseLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "PointDiffuse");
	parent->addChild(lightNode);

	this->serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	this->serializeToNode("Intensity", light->getIntensity(), lightNode);
	this->serializeShadow(light, lightNode);
}

void SceneWriter::serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);
}

void SceneWriter::serializeShadow(SceneLight* light, XmlTreeNode* lightNode){
	ShadowData* shadow = light->getShadowData();
	if(shadow->isActive()){
		lightNode->createAttribute("castShadow", "true");
		// Agrego la propiedades de la sombra y su atributo de prebanked
		XmlTreeNode* shadowNode = new XmlTreeNode("Shadow", lightNode);
		lightNode->addChild(shadowNode);
		string prebank = "false";
		if(shadow->isPrebanked()){
			prebank = "true";
		}
		shadowNode->createAttribute("prebanked", prebank);
		
		// Agrego la propiedad del shadowMap
		XmlTreeNode* shadowMapNode = new XmlTreeNode("ShadowMap", shadowNode);
		shadowNode->addChild(shadowMapNode);
		this->serializeToNode("Size", shadow->getShadowMapSize(), shadowMapNode);	
		this->serializeToNode("Perspective", shadow->getPerspectiveFovy(), shadowMapNode);	
		XmlTreeNode* shadowFilterNode = new XmlTreeNode("ShadowFilter", shadowNode);
		shadowNode->addChild(shadowFilterNode);
		this->serializeToNode("Factor", shadow->getPerspectiveFovy(), shadowFilterNode);	
	}
}

void SceneWriter::serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);

	XmlTreeNode* zNode = new XmlTreeNode("Z", StringUtils::toString(vector.z), parent);
	parent->addChild(zNode);
}

void SceneWriter::serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	this->serializeVec3ToNode(vector, propertyNode);
}

void SceneWriter::serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	this->serializeVec2ToNode(vector, propertyNode);
}

void SceneWriter::serializeToNode(string propertyName, float value, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode(propertyName, StringUtils::toString(value), parent);
	parent->addChild(xNode);
}