/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "MeshDrawerShader.h"
#include "ShaderManager.h"
#include "GraphicDevice.h"

MeshDrawerShader* MeshDrawerShader::evaluateAndChangeShader(vector<MeshDrawerShader*> *shaderHistory, vec3 centerPoint){
	
	ShaderManager* shaderManager = ShaderManager::getInstance();
	//Calculo la distancia
	this->setDistanceFactor(glm::length(centerPoint - shaderManager->getEyePoint()));
	if(shaderManager->optimizationsMustBeVerified()){		
		//Evaluo las posibilidades y retorno el shader correspondiente
		//Verifico si debo pasar a un shader mas básico
		// TODO: ESTO SE DEBE HACER UTILIZANDO UN BOUNDING BOX Y NO UN PUNTO CENTRAL (FALLA EN ESCENAS GRANDES)
		/*
		if(this->distanceFactor >= this->distanceToChangeShader){
			if(this->lowerShader != NULL){
				shaderHistory->push_back(this);
				return this->lowerShader;
			}
		}
		//Verifico si puedo volver al shader mas óptimo
		if(shaderHistory->size() > 0){
			MeshDrawerShader* previousShader = shaderHistory->back();
			if(this->distanceFactor < previousShader->getDistanceToChangeShader()){
				shaderHistory->pop_back();
				return previousShader;
			}
		}
		*/
	}
	//Me quedo con el shader actual
	return this;		
}

//Dibuja el contenido de un mesh
void MeshDrawerShader::draw(Mesh* mesh){
	// Cargo los uniforms y activo el programa de shader
	this->prepareForDraw(mesh);
	
	//Mando a renderizar
	GraphicDevice::getInstance()->drawMesh(mesh);
}

//Dibuja el contenido de un mesh utilizando transform feedback
void MeshDrawerShader::draw(Mesh* mesh, TransformFeedbackObject* feedbackHandle){
	// Cargo los uniforms y activo el programa de shader
	this->prepareForDraw(mesh);
	
	//Mando a renderizar utilizando el transformation feedback
	GraphicDevice::getInstance()->drawUsingTransformFeedback(mesh->getMeshBuffers()->getVaoObject(), feedbackHandle);
}