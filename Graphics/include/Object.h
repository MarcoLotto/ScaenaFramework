/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform2.hpp> 
#include <glm/gtx/projection.hpp>
#include "GLSLProgram.h"
#include "Mesh.h"
#include "Transformation.h"
#include "MeshDrawerShader.h"
#include <vector>
#include "ObjectLight.h"
using namespace std;

typedef glm::mat4 mat4;

//Contiene las propiedades basicas de cada objeto de la escena
//Los objetos de la escena deben extender de esta clase.
class Object{

protected:
	string id;  //Identifica al objeto por un id unico en la escena
	
	Mesh* mesh;
	string filename; // Esto no va aca, hay que hacer un refactor, lo pongo para simplificar laburo por ahora
	Transformation* transformation; // Contiene las transformaciones del objeto
	vector<ObjectLight*>* attachedLights;  // Las luces que contiene el objeto

	// Determina que objeto clonar (ej. un hijo de object)
	virtual Object* basicClone(string id);

public:
	Object(string id);
	virtual ~Object();

	//Devuelve todas las transformaciones del objeto
	Transformation* getTransformation(){ return this->transformation; }

	// Agrega luces al objeto
	void addLight(ObjectLight* light){ this->attachedLights->push_back(light); }

	// Consigue el vector de luces del objeto
	vector<ObjectLight*>* getAttachedLights(){ return this->attachedLights; }

	//Dibuja el objeto
	void draw(Camara* camera);
	//Dibuja el objeto aceptando una matriz de modelo externa (se mergea con la interna)
	void draw(mat4 modelMatrix, Camara* camera);
	//Dibuja el objeto utilizando un shader especifico
	void drawWithSpecificShader(MeshDrawerShader* programShader, Camara* camera);

	//Realiza el proceso de updae en este objeto
	virtual void update();

	//Setea el mesh asociado
	void setMesh(Mesh* mesh);
	Mesh* getMesh();

	// El id unico del objeto que identifica al objeto en una escena
	void setId(string id);
	string getId();

	// El filename de donde se cargo el objeto
	void setFilename(string filename){ this->filename = filename; }
	string getFilename(){ return this->filename; }

	// Devuelve un objeto clon de este (con otro id)
	virtual Object* clone(string id);
};