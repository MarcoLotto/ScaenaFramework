/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Mesh.h"
#include "MeshSkeleton.h"
#include "SkinnedAnimation.h"

class SkinnedMesh : public Mesh{
// RECORDAR QUE TODO LO QUE CAMBIA ACA TIENE QUE CAMBIAR EN EL CLONE DE SKINNED_MESH
private:
	MeshSkeleton* skeleton;
	vector<MeshSkeletonJoint*>* bindposeSkeletonByIndex;

	SkinnedAnimation* animation;
	SkinnedMesh* rootSkinnedMesh;

	SkinnedMesh* lastClonnedMesh; 

	bool drawingMesh;
	bool drawingSkeleton;

protected:
	// La llamada para decirle al shader activo que dibuje el mesh
	virtual void shaderDrawCall(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);
	void initDrawingOptions();
	void drawSkeleton(MeshDrawerShader* programShader, mat4 modelMatrix, mat4 viewMatrix, mat4 projMatrix);
	void mixJointsTransformationsAndBuildMatrix(mat4** finalPose, vector<MeshSkeletonJoint*>* actualPose);

public:
	SkinnedMesh();
	SkinnedMesh(MeshSkeleton* skeleton);
	virtual ~SkinnedMesh();	
	
	// Carga los buffers y demas cosas del mesh
	virtual void init();

	// Actualiza la transformacion del mesh
	virtual void update();

	virtual Mesh* clone();

	// Mismo addChild de mesh, pero si lo que se agrega es un skinned mesh, compruebo lo root mesh
	void addChild(SkinnedMesh* child);

	// No usar! Lo usa la arquitectura para borrar recursos compartidos en los meshes (como el esqueto, pero se borra en todos los meshes).
	virtual void deleteSharedResources();

	// Determina si se dibuja el mesh y el esqueleto (o alguno de los dos)
	void setDrawingMesh(bool value){ this->drawingMesh = value; }
	void setDrawingSkeleton(bool value){ this->drawingSkeleton = value; }

	bool isDrawingMesh(){ return this->drawingMesh; }
	bool isDrawingSkeleton(){ return this->drawingSkeleton; }

	// Asigno una animación para el esqueleto. La animacion se borra al dejar de usarse (sola).
	void setAnimation(SkinnedAnimation* animation);
	SkinnedAnimation* getAnimation(){ return this->animation; }

	// Setea una nueva animación pero partiendo de la animacion activa actualmente
	void setSmoothAnimation(SkinnedAnimation* animation);

	// Devuelve el esqueleto con que fue bindeado con el mesh
	MeshSkeleton* getBindposeSkeleton(){ return this->skeleton; }
	void setBindposeSkeleton(MeshSkeleton* skeleton){ this->skeleton = skeleton; skeleton->serializeForDrawOrderedByIndex(this->bindposeSkeletonByIndex); }
	unsigned int getJointsCount(){ return this->bindposeSkeletonByIndex->size(); }

	// El root skinned mesh es el padre de todos los skinned mesh de la jerarquia. Es el que tiene las animaciones.
	void setRootSkinnedMesh(SkinnedMesh* rootSkinnedMesh){ this->rootSkinnedMesh = rootSkinnedMesh; }
	void asignRootSkinnedMeshToChild(SkinnedMesh* child);

	// Referencia cual fue el ultimo mesh que clone (usado en la clonacion)
	SkinnedMesh* getLastClonnedMesh(){ return this->lastClonnedMesh; }

	// Guarda las transformaciones de cada joint del skeleton actual con respecto al skeleton de bindpose
	void getJointsFinalTransformations(mat4** destinationFinalPose);
};