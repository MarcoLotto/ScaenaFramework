/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ObjectLoader.h"
#include "XmlParser.h"
#include "MeshFactory.h"
#include "Logger.h"
#include "ShaderManager.h"
#include "Object.h"
#include "LightBuilder.h"
#include "StringUtils.h"
#include "WindowConnector.h"

vec3 ObjectLoader::getVec3FromNode(XmlTreeNode* node){
	vec3 vec = vec3(0.0f);
	//X
	XmlTreeNode* childNode = node->searchDirectChild("X", 0);
	if(childNode)
		vec.x = atof(childNode->getValue().c_str());
	//Y
	childNode = node->searchDirectChild("Y", 0);
	if(childNode)
		vec.y = atof(childNode->getValue().c_str());
	//Z
	childNode = node->searchDirectChild("Z", 0);
	if(childNode)
		vec.z = atof(childNode->getValue().c_str());
	return vec;
}

void serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);
}

// Escribe en el arbol la data del mapa displacement
void writeDisplacementMapToTree(Mesh* mesh, XmlTreeNode* parent){
	if(mesh->getDisplacementMap()->getMap() != NULL){
		XmlTreeNode* displacementNode = new XmlTreeNode("DisplacementMap", parent);
		parent->addChild(displacementNode);
	
		XmlTreeNode* mapNode = new XmlTreeNode("Map", mesh->getDisplacementMap()->getMapFilename(), displacementNode);
		displacementNode->addChild(mapNode);
	
		XmlTreeNode* factorNode = new XmlTreeNode("Factor", displacementNode);
		factorNode->setValue(StringUtils::toString(mesh->getDisplacementMap()->getDisplacementFactor()));
		displacementNode->addChild(factorNode);
	}
}

// Escribe en el arbol la data del mapa normal
void writeNormalMapToTree(Mesh* mesh, XmlTreeNode* parent){
	if(mesh->getNormalMap()->getMap() != NULL){
		XmlTreeNode* normalNode = new XmlTreeNode("NormalMap", parent);
		parent->addChild(normalNode);
	
		XmlTreeNode* mapNode = new XmlTreeNode("Map", mesh->getNormalMap()->getMapFilename(), normalNode);
		normalNode->addChild(mapNode);
	
		XmlTreeNode* factorNode = new XmlTreeNode("Factor", normalNode);
		factorNode->setValue(StringUtils::toString(mesh->getNormalMap()->getNormalFactor()));
		normalNode->addChild(factorNode);
	}
}

// Escribe en el arbol la data del mapa difuso
void writeDiffuseMapToTree(Mesh* mesh, XmlTreeNode* parent){
	if(mesh->getDiffuseMap()->getMap() != NULL){
		XmlTreeNode* diffuseNode = new XmlTreeNode("DiffuseMap", parent);
		parent->addChild(diffuseNode);
	
		XmlTreeNode* mapNode = new XmlTreeNode("Map", mesh->getDiffuseMap()->getMapFilename(), diffuseNode);
		diffuseNode->addChild(mapNode);
	}
}

XmlTreeNode* ObjectLoader::writeMeshToNode(Mesh* mesh, XmlTreeNode* parent){
	XmlTreeNode* baseNode = new XmlTreeNode("Mesh", parent);
	baseNode->createAttribute("name", mesh->getName());
		XmlTreeNode* materialNode = new XmlTreeNode("Material", baseNode);
		baseNode->addChild(materialNode);
			MeshMaterial* material = mesh->getMaterial();
			XmlTreeNode* ambientNode = new XmlTreeNode("Ambient", StringUtils::toString(material->getAmbient()), materialNode);
			materialNode->addChild(ambientNode);
			XmlTreeNode* difuseNode = new XmlTreeNode("Diffuse", StringUtils::toString(material->getDiffuse()), materialNode);
			materialNode->addChild(difuseNode);
			XmlTreeNode* specularNode = new XmlTreeNode("Specular", StringUtils::toString(material->getSpecular()), materialNode);
			materialNode->addChild(specularNode);
			XmlTreeNode* brightNode = new XmlTreeNode("Bright", StringUtils::toString(material->getBright()), materialNode);
			materialNode->addChild(brightNode);
		XmlTreeNode* textureNode = new XmlTreeNode("Texture", baseNode);
			baseNode->addChild(textureNode);
			XmlTreeNode* uvTranslationNode = new XmlTreeNode("UVTranslation", textureNode);
			textureNode->addChild(uvTranslationNode);
			serializeVec2ToNode(mesh->getUVTransformations()->getUvTranslation(), uvTranslationNode);
			XmlTreeNode* uvScaleNode = new XmlTreeNode("UVScale", textureNode);
			textureNode->addChild(uvScaleNode);
			serializeVec2ToNode(mesh->getUVTransformations()->getUvScale(), uvScaleNode);
			XmlTreeNode* uvRotationNode = new XmlTreeNode("UVRotation", textureNode);
			textureNode->addChild(uvRotationNode);
			serializeVec2ToNode(glm::vec2(0.0f, 0.0f), uvRotationNode);

			//Transformaciones
			serializeTransformationsToNode(mesh, baseNode);
			
			// Agrego el mapa difuso
			writeDiffuseMapToTree(mesh, textureNode);

			// Agrego el mapa normal
			writeNormalMapToTree(mesh, textureNode);

			// Agrego el mapa de displacement
			writeDisplacementMapToTree(mesh, textureNode);			
	return baseNode; 
 }

void ObjectLoader::writeLightToNode(ObjectLight* light, XmlTreeNode* node){
	int lightType = light->getLightType();		
	switch(lightType){
		case LIGHT_TYPE_POINT:
			serializePointLight(light, node);
			break;
		case LIGHT_TYPE_DIRECCIONAL:
			serializeDirectionalLight(light, node);
			break;
		case LIGHT_TYPE_SPOT:
			serializeSpotLight(light, node);
			break;
		case LIGHT_TYPE_POINT_DIFFUSE:
			serializePointDifuseLight(light, node);
			break;
	};
}

void ObjectLoader::serializePointLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Point");
	parent->addChild(lightNode);

	serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	serializeToNode("Intensity", light->getIntensity(), lightNode);
	serializeShadow(light, lightNode);
}

void ObjectLoader::serializeDirectionalLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Directional");
	parent->addChild(lightNode);

	serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	serializeToNode("Direction", light->getDirection(), lightNode);
	serializeToNode("Intensity", light->getIntensity(), lightNode);
	serializeShadow(light, lightNode);
}

void ObjectLoader::serializeSpotLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "Spot");
	parent->addChild(lightNode);

	serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	serializeToNode("Direction", light->getDirection(), lightNode);
	serializeToNode("Intensity", light->getIntensity(), lightNode);
	XmlTreeNode* factorNode = new XmlTreeNode("Factor", StringUtils::toString(light->getSpotFactor()), lightNode);
	lightNode->addChild(factorNode);
	serializeShadow(light, lightNode);
}

void ObjectLoader::serializePointDifuseLight(SceneLight* light, XmlTreeNode* parent){
	XmlTreeNode* lightNode = new XmlTreeNode("Light", parent);
	lightNode->createAttribute("type", "PointDiffuse");
	parent->addChild(lightNode);

	serializeToNode("Position", light->getAbsolutePosition(), lightNode);
	serializeToNode("Intensity", light->getIntensity(), lightNode);
	serializeShadow(light, lightNode);
}

void ObjectLoader::serializeShadow(SceneLight* light, XmlTreeNode* lightNode){
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
		serializeToNode("Size", shadow->getShadowMapSize(), shadowMapNode);	
		serializeToNode("Perspective", shadow->getPerspectiveFovy(), shadowMapNode);	
		XmlTreeNode* shadowFilterNode = new XmlTreeNode("ShadowFilter", shadowNode);
		shadowNode->addChild(shadowFilterNode);
		serializeToNode("Factor", shadow->getPerspectiveFovy(), shadowFilterNode);	
	}
}

void ObjectLoader::serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	serializeVec3ToNode(vector, propertyNode);
}

void ObjectLoader::serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	serializeVec2ToNode(vector, propertyNode);
}

void ObjectLoader::serializeToNode(string propertyName, float value, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode(propertyName, StringUtils::toString(value), parent);
	parent->addChild(xNode);
}

void ObjectLoader::writeFullMeshToNodes(XmlTreeNode* node, Mesh* mesh){
	for(unsigned int i=0; i < mesh->getChilds()->size(); i++){
		Mesh* childMesh = mesh->getChilds()->at(i);
		XmlTreeNode* childNode = writeMeshToNode(childMesh, node);
		node->addChild(childNode);
		writeFullMeshToNodes(childNode, childMesh);
	}
}

void ObjectLoader::writeAllLightsToNodes(XmlTreeNode* objectNode, vector<ObjectLight*>* objectLights){
	for(unsigned int i=0; i < objectLights->size(); i++){
		ObjectLight* light = objectLights->at(i);
		writeLightToNode(light, objectNode);
	}
}

XmlTree* ObjectLoader::objectToXlmTree(Object* object){
	XmlTree* xmlTree = new XmlTree();
	XmlTreeNode* node = xmlTree->getRootNode();
	XmlTreeNode* objectNode = new XmlTreeNode("Object", node);
	node->addChild(objectNode);
	objectNode->createAttribute("id", object->getId());
	objectNode->createAttribute("class", "Object");
	objectNode->createAttribute("meshPath", object->getMesh()->getFilePath());
	//Escribo el mesh asociado en nodos
	writeFullMeshToNodes(objectNode, object->getMesh());
	//Escribo las luces asociadas al objeto
	writeAllLightsToNodes(objectNode, object->getAttachedLights());
	return xmlTree;
}

XmlNodeAttribute* searchForAttribute(XmlTreeNode* node, string attributeName){
	for(unsigned int i=0; i<node->getAttributes().size(); i++){
		XmlNodeAttribute* attribute = node->getAttributes()[i];
		if(attribute->getName().compare(attributeName)==0)
			return attribute;
	}
	return NULL;
}

Object* initObject(XmlTreeNode* node){
	Object* object = NULL;
	XmlNodeAttribute* nameAttribute = searchForAttribute(node, "id");
	if(nameAttribute == NULL){
		Logger::getInstance()->logError(new Log("ObjectLoader#initObject: No se encontro el atributo id de Object")); 
		return NULL; //Error
	}		
	string objectId = nameAttribute->getValue();
	XmlNodeAttribute* classAttribute = searchForAttribute(node, "class");
	if(classAttribute == NULL){
		Logger::getInstance()->logError(new Log("ObjectLoader#initObject: No se encontro el atributo class del object " + objectId)); 
		return NULL; //Error
	}			
	XmlNodeAttribute* meshPathAttribute = searchForAttribute(node, "meshPath");
	if(meshPathAttribute == NULL){
		Logger::getInstance()->logError(new Log("ObjectLoader#initObject: No se encontro el atributo meshPath del object " + objectId)); 
		return NULL; //Error
	}
	//Creo el objecto del tipo especificado
	string className = classAttribute->getValue();
	if(className.compare("Object")==0) object = new Object(objectId);
	//INSTANCIO OTROS TIPOS DE OBJECT (DEFORMABLES, CON SHADERS ESPECIALES, ETC)
	else return NULL;

	//Cargo el mesh
	Mesh* mesh = MeshFactory::getInstance()->createMeshFromFile(meshPathAttribute->getValue().c_str());
	if(mesh == NULL){
		Logger::getInstance()->logError(new Log("ObjectLoader#initObject: No se pudo cargar el mesh " + meshPathAttribute->getValue())); 
		delete object;
		return NULL;
	}
	object->setMesh(mesh);	
	return object;
}

glm::vec2 getVec2FromNode(XmlTreeNode* propNode){
	XmlTreeNode* xNode = propNode->searchDirectChild("X", 0);
	XmlTreeNode* yNode = propNode->searchDirectChild("Y", 0);
	if((xNode != NULL)&&(yNode != NULL))
		return glm::vec2(atof(xNode->getValue().c_str()), atof(yNode->getValue().c_str()));
	return glm::vec2(0.0f);
}

void ObjectLoader::processMesh(Mesh* mesh, XmlTreeNode* node){
	XmlTreeNode* propNode = NULL;
	XmlTreeNode* childNode = NULL;
	//Los materiales
	XmlTreeNode* materialNode = node->searchDirectChild("Material", 0);
	if(materialNode != NULL){
		MeshMaterial* material = mesh->getMaterial();
		propNode = materialNode->searchDirectChild("Ambient", 0);
		if(propNode)
			material->setAmbient((float) atof(propNode->getValue().c_str()));
		propNode = materialNode->searchDirectChild("Diffuse", 0);
		if(propNode)
			material->setDiffuse((float) atof(propNode->getValue().c_str()));
		propNode = materialNode->searchDirectChild("Specular", 0);
		if(propNode)
			material->setSpecular((float) atof(propNode->getValue().c_str()));
		propNode = materialNode->searchDirectChild("Bright", 0);
		if(propNode)
			material->setBright((float) atof(propNode->getValue().c_str()));
	}

	//La textura
	XmlTreeNode* textureNode = node->searchDirectChild("Texture", 0);
	if(textureNode != NULL){
		propNode = textureNode->searchDirectChild("UVTranslation", 0);
		if(propNode)
			mesh->getUVTransformations()->setUvTranslation(getVec2FromNode(propNode));
		propNode = textureNode->searchDirectChild("UVScale", 0);
		if(propNode)
			mesh->getUVTransformations()->setUvScale(getVec2FromNode(propNode));
		propNode = textureNode->searchDirectChild("UVRotation", 0);
		//if(propNode)
		//	material->setUVRotation(getVec2FromNode(propNode));
		propNode = textureNode->searchDirectChild("DiffuseMap", 0);
		if(propNode){
			childNode = propNode->searchDirectChild("Map", 0);
			mesh->getDiffuseMap()->setMap(childNode->getValue());
		}
		propNode = textureNode->searchDirectChild("NormalMap", 0);
		if(propNode){
			childNode = propNode->searchDirectChild("Map", 0);
			mesh->getNormalMap()->setMap(childNode->getValue());
			childNode = propNode->searchDirectChild("Factor", 0);
			mesh->getNormalMap()->setNormalFactor((float) atof(childNode->getValue().c_str()));
		}
		propNode = textureNode->searchDirectChild("DisplacementMap", 0);
		if(propNode){
			childNode = propNode->searchDirectChild("Map", 0);
			mesh->getDisplacementMap()->setMap(childNode->getValue());
			childNode = propNode->searchDirectChild("Factor", 0);
			mesh->getDisplacementMap()->setDisplacementFactor((float) atof(childNode->getValue().c_str()));
		}		
	}

	//Cargo la matrix de transformacion del objeto a partir del xml
	XmlTreeNode* transfNode = node->searchDirectChild("Transformations", 0);
	if(transfNode != NULL){
		XmlTreeNode* translationNode = transfNode->searchDirectChild("Translation", 0);
		XmlTreeNode* rotationNode = transfNode->searchDirectChild("Rotation", 0);
		XmlTreeNode* scaleNode = transfNode->searchDirectChild("Scale", 0);
		if((!translationNode)||(!rotationNode)||(!scaleNode)){
			string message = "SceneLoaderError: Falta nodo de transformacion de mesh " + mesh->getName() + " se usa la default\n";
			Logger::getInstance()->logError(new Log(message));
		}
		Transformation* transformation = mesh->getTransformation();
		transformation->setPosition(getVec3FromNode(translationNode));
		transformation->setRotation(getVec3FromNode(rotationNode));
		transformation->setScale(getVec3FromNode(scaleNode));
	}
}

void ObjectLoader::loadMesh(Mesh* mesh, XmlTreeNode* node){
	// Primero buscamos si hay algun mesh que pertenezca a otro archivo de mesh, y en tal caso lo cargamos ya agremos al mesh padre
	for(int i=0; i < node->getChildNodes().size(); i++){
		XmlTreeNode* childNode = node->getChildNodes()[i];
		if(childNode->getName().compare("Mesh") == 0){
			XmlNodeAttribute* meshFilePathAtt = childNode->searchForAttribute("meshPath");
			if(meshFilePathAtt != NULL){
				Mesh* loadedMesh = MeshFactory::getInstance()->createMeshFromFile(meshFilePathAtt->getValue().c_str());
				if(mesh != NULL){
					// Agregamos todos los hijos del mesh cargado al mesh padre, eliminando del medio al mesh root
					for(int i=0; i < loadedMesh->getChilds()->size(); i++){
						mesh->addChild(loadedMesh->getChilds()->at(i));								
					}
				}
				else{
					Logger::getInstance()->logError(new Log("ObjectLoader#loadMesh: No se pudo cargar el mesh " + meshFilePathAtt->getValue()));
				}					
			}
		}
	}
	// Ahora por cada hijo del mesh padre, buscamos su correspondiente definicion y ajustamos sus propiedades
	for(unsigned int i=0; i < mesh->getChilds()->size(); i++){
		Mesh* childMesh = mesh->getChilds()->at(i);
		XmlTreeNode* childNode = node->searchDirectChild("Mesh", i);
		if(childNode != NULL){
			XmlNodeAttribute* nameAttribute = childNode->searchForAttribute("name");
			XmlNodeAttribute* meshFilePathAtt = childNode->searchForAttribute("meshPath");
			if(!meshFilePathAtt && !nameAttribute)
				continue;  
			if(nameAttribute->getValue().compare(childMesh->getName())==0){
				processMesh(childMesh, childNode);
				loadMesh(childMesh, childNode);
			}
			else
				Logger::getInstance()->logDebug(new Log("ObjectLoader#loadMesh: Mesh name " + mesh->getName() + " difiere del node name " + childNode->getName()));
		}
	}	
	// Reevaluo que shader se adapta mejor al mesh	
	mesh->setShaderProgram(ShaderManager::getInstance()->reevaluateTheBestShader(mesh));	
}

vec3 ObjectLoader::getVec3LightParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("ObjectLoader#getVec3LightParam: Falta nodo de " + paramName + " en un Light, se usa el parametro por default(cero)"));
		return vec3(0.0f);
	}
	return getVec3FromNode(paramNode);
}

float ObjectLoader::getFloatLightParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("ObjectLoader#getFloatLightParam: Falta nodo de " + paramName + " en un Light, se usa el parametro por default(cero)"));
		return 0.0f;
	}
	return atof(paramNode->getValue().c_str());
}

ObjectLight* ObjectLoader::processSpotLight(Object* object, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 direction = getVec3LightParam("Direction", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);
	float factor = getFloatLightParam("Factor", lightNode);
	//Genero la luz
	return LightBuilder::buildSpotObjectLight(object, position, direction, intensity, factor);
}

ObjectLight* ObjectLoader::processPointLight(Object* object, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	return LightBuilder::buildPointObjectLight(object, position, intensity);
}

ObjectLight* ObjectLoader::processPointDiffuseLight(Object* object, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	return LightBuilder::buildPointObjectLight(object, position, intensity);
}

ObjectLight* ObjectLoader::processDirectionalLight(Object* object, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 direction = getVec3LightParam("Direction", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	ObjectLight* light = LightBuilder::buildDireccionalObjectLight(object, direction, intensity);
	light->setLocalPosition(position);  //Para la sombra (si tiene)
	return light;
}

vec2 ObjectLoader::getVec2FromNode2(XmlTreeNode* node){
	vec2 vec = vec2(0.0f);
	//X
	XmlTreeNode* childNode = node->searchDirectChild("X", 0);
	if(childNode)
		vec.x = atof(childNode->getValue().c_str());
	//Y
	childNode = node->searchDirectChild("Y", 0);
	if(childNode)
		vec.y = atof(childNode->getValue().c_str());
	return vec;
}

vec2 ObjectLoader::getVec2ShadowParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("ObjectLoader#getVec2ShadowParam: Falta nodo de "+ paramName+" en un Shadow, se usa el parametro por default(cero)"));
		return vec2(0.0f);
	}
	return getVec2FromNode2(paramNode);
}

float ObjectLoader::getFloatParam(string paramName, XmlTreeNode* node){ 
	XmlTreeNode* paramNode = node->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("ObjectLoader#getFloatParam: Falta nodo de "+paramName+", se usa el parametro por default(cero)"));
		return 0.0f;
	}
	return atof(paramNode->getValue().c_str());
}

void ObjectLoader::processShadow(XmlTreeNode* lightNode, ObjectLight* light){
	XmlTreeNode* shadowNode = lightNode->searchDirectChild("Shadow", 0);
	if(!shadowNode){
		Logger::getInstance()->logDebug(new Log("ObjectLoader#processShadow: Falta el nodo \"Shadow\" en etiqueta Light definida como \"castShadow\", se ignora la sombra"));
		return;
	}
	XmlTreeNode* shadowMapNode = shadowNode->searchDirectChild("ShadowMap", 0);
	if(!shadowMapNode){
		Logger::getInstance()->logDebug(new Log("ObjectLoader#processShadow: Falta el nodo \"ShadowMap\" en etiqueta Shadow, se ignora la sombra"));
		return;
	}
	//El tamaño de shadowMap
	ShadowData* shadow = light->getShadowData(); 
	vec2 size = getVec2ShadowParam("Size", shadowMapNode);
	shadow->setShadowMapSize(size);
	//La perspectiva fovy del shadowMap
	float perspectiveFovy = getFloatParam("Perspective", shadowMapNode);
	shadow->setPerspectiveFovy(perspectiveFovy);

	//El factor del filtro de shadow si es que hay
	XmlTreeNode* filterNode = lightNode->searchDirectChild("ShadowFilter", 0);
	if(filterNode != NULL){
		float filterFactor = getFloatParam("Factor", filterNode);
		shadow->setFilterFactor(filterFactor);
	}
	XmlNodeAttribute* preBankedAtt = shadowNode->searchForAttribute("prebanked");
	//Es si prebanqueada, la seteo como tal
	if((preBankedAtt)&&(preBankedAtt->getValue().compare("true")==0)){
		shadow->setPrebanked(true);
	}
	//Activo la sombra (falta activarla realmente cuando se agrega el objeto a la escena)
	shadow->setActive(true);	
}


void ObjectLoader::loadLights(Object* object, XmlTreeNode* objectNode){
	for(int i=0; i < objectNode->getChildNodes().size(); i++){
		XmlTreeNode* lightNode = objectNode->getChildNodes()[i];
		if(lightNode->getName().compare("Light")==0){
			XmlNodeAttribute* typeAttribute = lightNode->searchForAttribute("type");
			if(!typeAttribute){
				Logger::getInstance()->logDebug(new Log("ObjectLoader#loadLights: Falta atributo \"type\" en etiqueta Light, se ignora la luz"));
				return;
			}
			//Busco toda la informacion de la luz			
			//Dependiendo del tipo de luz consigo los parametros correspondientes
			ObjectLight* light = NULL;
			string lightType = typeAttribute->getValue();
			if(lightType.compare("Spot")==0) 
				light = processSpotLight(object, lightNode);
			else if(lightType.compare("Point")==0) 
				light = processPointLight(object, lightNode);
			else if(lightType.compare("PointDiffuse")==0) 
				light = processPointDiffuseLight(object, lightNode);
			else if(lightType.compare("Directional")==0) 
				light = processDirectionalLight(object, lightNode);
			else{
				Logger::getInstance()->logDebug(new Log("ObjectLoader#loadLights: Atributo \"type\" invalido, se ignora la luz"));
				return;
			}

			//Cargo la sombra si es que hay
			XmlNodeAttribute* shadowAtt = lightNode->searchForAttribute("castShadow");
			if((shadowAtt)&&(shadowAtt->getValue().compare("true")==0)){
				processShadow(lightNode, light);
			}
			// Agrego la luz al objeto
			object->addLight(light);
		}
	}	
}

Object* ObjectLoader::loadObjectFromFile(const char* filename){
	Object* object = NULL;
	//Leo el archivo y lo parseo en un arbol
	XmlTree* xmlTree = XmlParser::parse(string(WindowConnector::getBaseApplicationPath() + filename).c_str());
	//***Transformo el arbol en un Object***
	XmlTreeNode* root =  xmlTree->getRootNode();
	//Primiero busco la etiqueta de Object
	XmlTreeNode* objectNode = NULL;
	for(unsigned int i=0; i < root->getChildNodes().size(); i++){
		objectNode = root->getChildNodes()[i];
		if(objectNode->getName().compare("Object")==0){	
			//Cargo el mesh y le seteo los valores del xml
			object = initObject(objectNode);
			break;
		}
	}
    if(object == NULL){
        if(xmlTree != NULL){ delete xmlTree; }
		return NULL; //Error
    }

	//Busco las etiquetas de Mesh y reemplazo los valores del xml
	Mesh* mesh = object->getMesh();
	if(mesh->getChilds()->size() > 0){		
		loadMesh(mesh, objectNode);		
	}
	//Busco las etiquetas de Light y las agrego a las luces del objeto
	loadLights(object, objectNode);

	//Guardo el path en el object
	object->setFilename(filename);

    delete xmlTree;
	return object;
}

void ObjectLoader::serializeTransformationsToNode(Mesh* mesh, XmlTreeNode* parentNode){
	XmlTreeNode* transformationNode = new XmlTreeNode("Transformations", parentNode);
	parentNode->addChild(transformationNode);
		// Las propiedades hijas
		Transformation* transformation = mesh->getTransformation();
		serializeToNode("Translation", transformation->getPosition(), transformationNode);
		serializeToNode("Rotation", transformation->getRotation(), transformationNode);
		serializeToNode("Scale", transformation->getScale(), transformationNode);
}

void ObjectLoader::serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);

	XmlTreeNode* zNode = new XmlTreeNode("Z", StringUtils::toString(vector.z), parent);
	parent->addChild(zNode);
}

void ObjectLoader::serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);
}
