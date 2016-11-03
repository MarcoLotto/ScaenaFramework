/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Mesh.h"
#include "MeshTransformFeedbackShader.h"
#include "TransformFeedbackObject.h"

class TransformableMesh : public Mesh{

private:	
	// El transformBuffer entrega la informacion de entrada al shader de transformacion
	MeshBuffers* transformBuffer;
	// El feedbackBuffer se utilizará para almacenar la salida de la transformacion
	MeshBuffers* feedbackBuffer;

	// Identifica que shader se utilizara para transformar
	MeshTransformFeedbackShader* transformShader;  
	
protected:
	// La llamada para decirle al shader activo que dibuje el mesh
	virtual void shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);

public:
	TransformableMesh();
	virtual ~TransformableMesh();
	
	// Carga los buffers y demas cosas del mesh
	virtual void init();

	// Actualiza la transformacion del mesh
	virtual void update();
	
	// Asigna el shader de transformación a solo este mesh, no a sus hijos (podrian no ser transformables)
	void setTransformShaderOnlyToThisMesh(MeshTransformFeedbackShader* transformShader);	
	MeshTransformFeedbackShader* getTransformShader(){ return this->transformShader; }
};