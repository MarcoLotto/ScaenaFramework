/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SceneLoader.h"
#include <stdio.h>	
#include "XmlParser.h"
#include "SceneLoader.h"
#include "ObjectLoader.h"
#include "Object.h"
#include "WindowConnector.h"
#include "Logger.h"

vec3 getVec3FromNode(XmlTreeNode* node){
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

vec2 getVec2FromNode2(XmlTreeNode* node){
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

bool parseObject(Scene* scene, XmlTreeNode* sceneNode){
	for(int i=0; i < sceneNode->getChildNodes().size(); i++){
		XmlTreeNode* objectNode = sceneNode->getChildNodes()[i];
		if(objectNode->getName().compare("Object")==0){
			XmlNodeAttribute* pathAttribute = objectNode->searchForAttribute("path");
			if(!pathAttribute){
				Logger::getInstance()->logError(new Log("SceneLoaderError:Falta atributo \"path\" en etiqueta Object, se ignora el objecto"));				
				return false;
			}
			string filename = pathAttribute->getValue();
			// Cargo el objeto desde el archivo
			Object* object = ObjectLoader::loadObjectFromFile(pathAttribute->getValue().c_str());
			if(object == NULL){
				Logger::getInstance()->logError(new Log("SceneLoaderError:No se encontro el archivo de objeto " + filename + " o no se pudo cargar alguno de sus componenetes"));
				return false;
			}
			// Me guardo el path del objeto
			object->setFilename(filename);

			//Cargo la matrix de transformacion del objeto a partir del xml
			XmlTreeNode* transfNode = objectNode->searchDirectChild("Transformations", 0);
			if(transfNode != NULL){
				XmlTreeNode* translationNode = transfNode->searchDirectChild("Translation", 0);
				XmlTreeNode* rotationNode = transfNode->searchDirectChild("Rotation", 0);
				XmlTreeNode* scaleNode = transfNode->searchDirectChild("Scale", 0);
				if((!translationNode)||(!rotationNode)||(!scaleNode)){
					Logger::getInstance()->logError(new Log("SceneLoaderError:Falta nodo de transformacion de Object " + pathAttribute->getValue() + ", se ignora el objecto\n"));
					return false;
				}
				Transformation* transformation = object->getTransformation();
				transformation->setPosition(getVec3FromNode(translationNode));
				transformation->setRotation(getVec3FromNode(rotationNode));
				transformation->setScale(getVec3FromNode(scaleNode));

				//Agrego el objecto a la escena
				scene->getObjectManager()->addObject(object);
			}
		}
	}
	return true;
}

float getFloatLightParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("SceneLoaderError:Falta nodo de \"" + paramName + "\" en un Light, se usa el parametro por default(cero)\n"));
		return 0.0f;
	}
	return atof(paramNode->getValue().c_str());
}

vec3 getVec3LightParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("SceneLoaderError:Falta nodo de \"" + paramName + "\" en un Light, se usa el parametro por default(cero)"));
		return vec3(0.0f);
	}
	return getVec3FromNode(paramNode);
}
vec2 getVec2ShadowParam(string paramName, XmlTreeNode* lightNode){ 
	XmlTreeNode* paramNode = lightNode->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("SceneLoaderError:Falta nodo de \"" + paramName + "\" en un Shadow, se usa el parametro por default(cero)"));		
		return vec2(0.0f);
	}
	return getVec2FromNode2(paramNode);
}

float getFloatParam(string paramName, XmlTreeNode* node){ 
	XmlTreeNode* paramNode = node->searchDirectChild(paramName, 0);
	if(!paramNode){
		Logger::getInstance()->logError(new Log("SceneLoaderError:Falta nodo de \"" + paramName + "\", se usa el parametro por default(cero)\n"));		
		return 0.0f;
	}
	return atof(paramNode->getValue().c_str());
}

void processShadow(Scene* scene, XmlTreeNode* lightNode, SceneLight* light){
	XmlTreeNode* shadowNode = lightNode->searchDirectChild("Shadow", 0);
	if(!shadowNode){
		Logger::getInstance()->logError(new Log("SceneLoaderWarning:Falta el nodo \"Shadow\" en etiqueta Light definida como \"castShadow\", se ignora la sombra"));
		return;
	}
	XmlTreeNode* shadowMapNode = shadowNode->searchDirectChild("ShadowMap", 0);
	if(!shadowMapNode){
		Logger::getInstance()->logError(new Log("SceneLoaderWarning:Falta el nodo \"ShadowMap\" en etiqueta Shadow, se ignora la sombra"));
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
	//Activo la sombra
	scene->getLightingManager()->castShadow(true, light);
	XmlNodeAttribute* preBankedAtt = shadowNode->searchForAttribute("prebanked");
	//Es si prebanqueada, la seteo como tal
	if((preBankedAtt)&&(preBankedAtt->getValue().compare("true")==0))
		shadow->setPrebanked(true);
}

SceneLight* processSpotLight(Scene* scene, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 direction = getVec3LightParam("Direction", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);
	float factor = getFloatLightParam("Factor", lightNode);
	//Genero la luz
	return scene->getLightingManager()->setSpotLight(position, direction, intensity, factor);
}

SceneLight* processPointLight(Scene* scene, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	return scene->getLightingManager()->setPointLight(position, intensity);
}

SceneLight* processPointDiffuseLight(Scene* scene, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	return scene->getLightingManager()->setPointLightOnlyDiffuse(position, intensity);
}

SceneLight* processDirectionalLight(Scene* scene, XmlTreeNode* lightNode){
	vec3 position = getVec3LightParam("Position", lightNode);
	vec3 direction = getVec3LightParam("Direction", lightNode);
	vec3 intensity = getVec3LightParam("Intensity", lightNode);	
	//Genero la luz
	SceneLight* light = scene->getLightingManager()->setDireccionalLight(direction, intensity);
	light->setAbsolutePosition(position);  //Para la sombra (si tiene)
	return light;
}


void parseLight(Scene* scene, XmlTreeNode* sceneNode){
	LightingManager* lightingManager = scene->getLightingManager();
	for(int i=0; i < sceneNode->getChildNodes().size(); i++){
		XmlTreeNode* lightNode = sceneNode->getChildNodes()[i];
		if(lightNode->getName().compare("Light")==0){
			XmlNodeAttribute* typeAttribute = lightNode->searchForAttribute("type");
			if(!typeAttribute){
				Logger::getInstance()->logError(new Log("SceneLoaderError:Falta atributo \"type\" en etiqueta Light, se ignora la luz"));
				return;
			}
			//Busco toda la informacion de la luz			
			//Dependiendo del tipo de luz consigo los parametros correspondientes
			SceneLight* light = NULL;
			string lightType = typeAttribute->getValue();
			if(lightType.compare("Spot")==0) 
				light = processSpotLight(scene, lightNode);
			else if(lightType.compare("Point")==0) 
				light = processPointLight(scene, lightNode);
			else if(lightType.compare("PointDiffuse")==0) 
				light = processPointDiffuseLight(scene, lightNode);
			else if(lightType.compare("Directional")==0) 
				light = processDirectionalLight(scene, lightNode);
			else{
				Logger::getInstance()->logError(new Log("SceneLoaderError:Atributo \"type\" invalido, se ignora la luz"));
				return;
			}

			//Cargo la sombra si es que hay
			XmlNodeAttribute* shadowAtt = lightNode->searchForAttribute("castShadow");
			if((shadowAtt)&&(shadowAtt->getValue().compare("true")==0)){
				processShadow(scene, lightNode, light);
			}
		}
	}	
}

Scene* SceneLoader::loadFromXmlFile(const char* filename){
	Scene* scene = NULL;
	//Leo el archivo y lo parseo en un arbol
	XmlTree* xmlTree = XmlParser::parse(string(WindowConnector::getBaseApplicationPath() + filename).c_str());
	//***Transformo el arbol en un Scene***
	XmlTreeNode* root =  xmlTree->getRootNode();
	//Primiero busco la etiqueta de Scene
	XmlTreeNode* sceneNode = NULL;
	for(int i=0; i < root->getChildNodes().size(); i++){
		sceneNode = root->getChildNodes()[i];
		if(sceneNode->getName().compare("Scene")==0){	
			//Cargo el mesh y le seteo los valores del xml
			scene = new Scene();
			break;
		}
	}
    if(scene == NULL){
        if(xmlTree != NULL){ delete xmlTree; }
        return NULL; //Error
    }

	//Busco las etiquetas de Object y creo los objectos (si hay error, devuelvo null)
	if(!parseObject(scene, sceneNode)){
		delete scene;
		return NULL;
	}
	//Busco las etiquetas de Light y creo las luces correspondientes
	parseLight(scene, sceneNode);
	//Busco las etiquetas de Camera y creo las camaras

    delete xmlTree;
	return scene;
}