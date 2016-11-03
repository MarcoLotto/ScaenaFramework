/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TransformableMesh.h"
#include "GraphicDevice.h"

TransformableMesh::TransformableMesh() : Mesh(){	
	this->transformBuffer = NULL;
	this->feedbackBuffer = NULL;
	this->transformShader = NULL;
}

TransformableMesh::~TransformableMesh(){	
	if(this->feedbackBuffer != NULL){
		delete this->feedbackBuffer;
	}
}
	
void TransformableMesh::init(){
	// No quiero que me inicialize dos veces
	if(this->alreadyInitialized)
		return;
	this->alreadyInitialized = true;

	// Primero mando a cargar en memoria de video los buffers (si ya estan cargados no se hara denuevo)
	this->getMeshBuffers()->load();

	// Mando a calcular el bounding
	this->bounding->recalculate(this->meshBuffers->getVertexBufferObject());

	// Me guardo el meshBuffer original para siempre partir transformando de lo mismo	
	this->feedbackBuffer = this->meshBuffers;

	// Guardo memoria para el resultado de la transformacion. Esto lo hago clonando el buffer original (todavia no se copia) y volviendo a cargarlo.
	this->transformBuffer = new MeshBuffers();
	this->feedbackBuffer->cloneUninitialized(this->transformBuffer);
	this->transformBuffer->setIndexBuffer(NULL, 0);
	this->transformBuffer->load();

	// Si hay un shader de transformacion asignado, declaro los VBOs de output
	if(this->transformShader != NULL){
		this->transformShader->declareFeedbackInputsAndOutputs(this->transformBuffer, this->feedbackBuffer);
	}
		
	//Mando a cargar los hijos
	std::vector<Mesh*>::iterator it = this->getChilds()->begin();
	while(it != this->getChilds()->end()){
		(*it)->init();
		++it;
	}
}

void TransformableMesh::update(){
	if(this->transformShader != NULL){
		//Proceso la transformacion
		this->transformShader->execute();	

		// Me guardo en el meshBuffer de este mesh el resultado de la transformacion (asi la puedo dibujar)
		this->meshBuffers = this->feedbackBuffer;
	}
	//Mando a hacer update a los hijos
	for(int i=0; i < this->childs->size(); i++){
		this->childs->at(i)->update();
	}
}

// La llamada para decirle al shader activo que dibuje el mesh
void TransformableMesh::shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix){
	// Esta llamada, utiliza el index buffer del mesh original para dibujar
	programShader->draw(this);

	// Si se utiliza esta llamada se utiliza el conteo de vertices del transform feedback. Utilizar en casos de no contar con index buffer o no saber la cantidad de vertices creados.
	//programShader->draw(this, modelMatrix, viewMatrix, projMatrix, this->transformShader->getTransformFeedbackObject());
}

void TransformableMesh::setTransformShaderOnlyToThisMesh(MeshTransformFeedbackShader* transformShader){
	// Guardo el transform shader
	this->transformShader = transformShader;

	// Si el mesh ya fue inicializado, declaro los outputs en el tranmsform shader
	if(this->alreadyInitialized)
		this->transformShader->declareFeedbackInputsAndOutputs(this->transformBuffer, this->feedbackBuffer);
}
