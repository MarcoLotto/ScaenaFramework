/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <vector>
#include "MeshDrawerShader.h"
#include "Scene.h"

//ATENCION: este delta_distance indica la minima variacion en la posicion del ojo para que se
//compruebe si es necesario cambiar de shaders. Valores demasiado grandes o demasiado chicos
//podrian perjudicar la performance del sistema
#define DELTA_DISTANCE 1.0f

/***************************************************************************************
************************************ShaderManager***************************************
*Administra todos los shaders del programa. Los shaders le piden instancias de otros
*cuando lo creen conveniente. Para un determinado mesh es capaz de decir que shader es 
*el mas conveniente. Es un singleton por lo que se puede instanciar desde cualquier lado.
****************************************************************************************/
class ShaderManager{

private:
	static ShaderManager* instance;
	vector<GLSLProgram*> shaderPrograms;
	vec3 lastPosition;
	vec3 actualPosition;
	bool isOptimizationNeeded;
	Scene* currentScene;  // Algunos shaders necesitan datos de la escena. Se los provee el shader manager.

	void configure();
	GLSLProgram* getShaderByName(const char* name);
	
public:
	static ShaderManager* getInstance();
	//Compila y linkea todos los shaders disponibles
	void initAllShaders();
	//Devuelve el shader cuyo ID es shaderId
	GLSLProgram* getShaderInstance(int shaderId);
	//Evalua las propiedades del mesh y devuelve el shader que mas se ajusta. Reevaluate hace lo mismo pero en base al shader que ya tenia el mesh.
	MeshDrawerShader* evaluateTheBestShader(Mesh* mesh, bool hasSkeleton);
	MeshDrawerShader* reevaluateTheBestShader(Mesh* mesh);
	//Hace un update de los datos del shader manager. Esto debe ejecutarse UNA vez por frame
	void update(vec3 eyePosition);
	//Indica si se deben evaluar optimizaciones o no hace falta. Se puede ejecutar muchas veces por frame, no tiene gran consumo.
	bool optimizationsMustBeVerified();
	//Devuelve el punto del ojo en el momento actual
	vec3 getEyePoint();

	// Setea que estrategia de renderizado utilizar (deferred, forwarding, etc)
	void useDeferredShadingStrategy();
	void useForwardShadingStrategy();

	void setCurrentScene(Scene* scene){ this->currentScene = scene; }
	Scene* getCurrentScene(){ return this->currentScene; }

};