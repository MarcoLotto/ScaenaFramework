/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "Object.h"
#include <vector>

class LightingManager;

class ObjectManager{

private:
	vector<Object*> objectContainer;
	int posicionesLibres;  //Indica la cantidad de posiciones libres en el medio del vector
	LightingManager* lightingManager; // Guardo una referencia al lightingManager, porque los objetos tambien tienen luces atacheadas

	//Remueve el objeto (no borra luces), pero no lo borra de la memoria, sino que lo devuelve (pop). Devuelve null si la posicion no es valida
	Object* popObject(int positionInContainer);

public:	
	ObjectManager();
	virtual ~ObjectManager();

	// Tener seteado un lighting manager es necesario porque los objetos tambien pueden tener luces
	void setLightingManager(LightingManager* lightingManager){ this->lightingManager = lightingManager; }

	//Hace un update de todos los objetos
	void update();

	//Renderiza todo el contenido
	void render(Camara* camera);
	//Renderiza la escena con un shader especifico (utilizado por ej. al generar sombras)
	void renderWithSpecificShader(MeshDrawerShader* shaderProgram, Camara* camera);

	//Agrega un object ya inicializado y devuelve su posicion en el container
	int addObject(Object* object);

	//Devuelve el objeto a partir de la posicion en el container
	Object* getObjectByPosition(int positionInContainer);
	//Devuelve la posicion en el container a partir de la referencia al objeto
	int getPositionByObject(Object* object);
	//Devuelve la posicion del objeto en el container a partir del id unico del objeto
	int getPositionByObjectId(string id);

	//Es la forma para recorrer la lista de objetos. Va devolviendo objetos a medida que incremento el index.
	//Devuelve NULL cuando no hay mas objetos.
	Object* getObjectByIteration(int index);

	//Remueve un objeto y sus luces asociadas (pero no lo borra fisicamente)
	void removeObject(Object* object);
	//Idem anterior pero mas rapido (no hace falta buscar al objeto). Devuelve el objeto borrado.
	Object* removeObject(int positionInContainer);

	// Remueve todos los objetos y sus luces asociadas (pero no los borra fisicamente)
	void clear();

	// Se hace una copia exacta del manager y de sus objetos. Se debe indicar que lighting manager se utilizará.
	ObjectManager* clone(LightingManager* lightingManager);

	// Borra fisicamente todos los objetos (no se borran sus luces asociadas, para esto esta el lightingManager)
	void deleteAllObjects();
};
