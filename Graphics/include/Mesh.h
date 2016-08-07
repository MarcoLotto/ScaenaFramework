/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <glm/glm.hpp> 
#include "MeshDrawerShader.h"
#include "MeshMaterial.h"
#include "Transformation.h"
#include "NormalMap.h"
#include "DisplacementMap.h"
#include "DiffuseMap.h"
#include "MeshBuffers.h"
#include "UVTransformations.h"
#include "BoundingBox.h"
#include "Camara.h"
#include "LineR3.h"
#include "DrawPacket.h"

#include <vector>
using namespace std;

class Mesh{
// RECORDAR QUE TODO LO QUE CAMBIA ACA TIENE QUE CAMBIAR EN EL CLONE DE MESH
protected:
	MeshDrawerShader* shaderProgram;
	string name;
	string filePath;
	bool alreadyInitialized;
	bool isDrawDataDirty;

	// La lista de los mesh hijos y la historia de shaders que pasaron
	std::vector<Mesh*>* childs;
	std::vector<MeshDrawerShader*> shaderHistory;

	// Inicializa y borra los recursos del mesh
	void initiateResources();
	void deleteResources();	

	// El parametro needRecalculation indica si es necesario que se recalcule la matriz de modelo
	virtual void draw(mat4 parentGlobalModelMatrix, Camara* camera, bool needRecalculation);
	virtual void drawWithSpecificShader(MeshDrawerShader* programShader, mat4 parentGlobalModelMatrix, Camara* camera, bool needRecalculation);
	void internalPreloadDrawData(mat4 modelMatrix, Camara* camera, bool needRecalculation, bool childsRecalculation);

	// La llamada para decirle al shader activo que dibuje el mesh
	virtual void shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);		

	// Los recursos
	MeshMaterial* meshMaterial;
	Transformation* transformation;
	DiffuseMap* diffuseMap;
	NormalMap* normalMap;
	DisplacementMap* displacementMap;
	MeshBuffers* meshBuffers;
	UVTransformations* uvTransformations;
	BoundingBox* bounding;

	// La metadata para el dibujado
	DrawPacket* drawPacket;
		
public:
	Mesh();
	virtual ~Mesh();

	void setName(string name){ this->name = name; }
	string getName(){ return this->name; }

	void addChild(Mesh* child){ this->childs->push_back(child); }
	vector<Mesh*>* getChilds(){ return this->childs; }
	
	void setShaderProgram(MeshDrawerShader* shader);
	MeshDrawerShader* getShaderProgram();
		
	void setFilePath(string filename){ this->filePath = filename; }
	string getFilePath(){ return this->filePath; }

	// Los accessors para los recursos, recordar que ya estan creados, por lo que no hay setters
	Transformation* getTransformation(){ return this->transformation; }
	MeshMaterial* getMaterial(){ return this->meshMaterial; }
	DiffuseMap* getDiffuseMap(){ return this->diffuseMap; }
	NormalMap* getNormalMap(){ return this->normalMap; }
	DisplacementMap* getDisplacementMap(){ return this->displacementMap; }
	MeshBuffers* getMeshBuffers(){ return this->meshBuffers; }
	UVTransformations* getUVTransformations(){ return this->uvTransformations; }
	BoundingBox* getBounding(){ return this->bounding; }

	// El paquete con la informacion de renderizado
	DrawPacket* getDrawPacket(){ return this->drawPacket; }

	// Calcula el bounding box que contiene a todos los boundings propios y de sus hijos
	void getGeneralBounding(BoundingBox* generalBounding);

	// Para encontrar un mesh por su nombre en la jerarquia, para borrarlo, o para hacer un swap con otro mesh. 
	// Encuentra un mesh: Si hay mas de un mesh en la jerarquia devuelve el que esta mas alto en la misma. Si no encuentra nada, retorna NULL.
	Mesh* findSubMesh(string name);
	// Borra un mesh: Lo borra realmente de la memoria. Si hay mas de un mesh con el mismo nombre, solo borra uno, el que mas alto este en la jerarquia. False si no encuentra nada.
	bool deleteSubMesh(string name);
	// Swap de mesh: Solo swapea uno, el que mas alto este en la jerarquia. False si no encuentra nada. El mesh original (por el que se hizo swap) es borrado.
	bool swapSubMesh(string nameToRemove, Mesh* meshToInsert);

	// Hace una copia del mesh y todos sus recursos (algunos son compartidos pero no afectan)
	virtual Mesh* clone();
	void clone(Mesh* clone);  // Cuidado con usar este! Solo clonará los atributos de un mesh común. El clone() clona todos los atributos de un mesh especializado (como skinnedMesh)
		
	//Deberian implementarse en una clase hija
	virtual void init();
	virtual void draw(mat4 modelMatrix, Camara* camera);
	virtual void drawWithSpecificShader(MeshDrawerShader* programShader, mat4 modelMatrix, Camara* camera);
	virtual void update();
	virtual void deleteSharedResources(){};  // Redefinir pero, no usar! Lo usa la arquitectura para borrar recursos compartidos en los meshes.

	// Sirve para precalcular la informacion de dibujado (es opcional pero daria mayor performance usando multithreading)
	void preloadDrawData(mat4 modelMatrix, Camara* camera);

	// Verifica si el mesh colisiona con otro
	bool isColliding(Mesh* otherMesh);
	// Verifico si el mesh colisiona con una recta. El segundo metodo es igual al primero pero permite identificar todos los puntos de interseccion con la recta
	bool isIntersecting(LineR3* line);
	bool isIntersecting(LineR3* line, list<vec3>* lineIntersectionPoints);
};