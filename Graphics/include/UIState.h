/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp> 
#include "UIShader.h"
#include "Texture.h"
#include "UIEventHandler.h"
#include <list>
using namespace std;

typedef glm::vec2 vec2;

class UIElement;

//******************************************************************************
//* Lleva un estado de un UIElement. Decide que dibujar y permite animaciones  *
//******************************************************************************
class UIState{

protected:
	UIShader* shader;
	void initBasicParameters();

	// Se redefine en clases hijas, se utiliza para actualizar el estado
	virtual void particularUpdate(UIState* finalStateBasicParameters);

	// De define la forma de dibujado para este state en particular
	virtual void particularDraw();

	// Actualiza la matriz de transformacion del la escena
	void generateTransformationMatrix(UIState* finalStateBasicParameters);

	//Si es un clon, esto identifica cual es el objeto original
	UIState* referenceToOriginal;

	//Contiene todos los elementos dibujables y clickeables del estado
	list<UIElement*> drawingElements;

	//Atributos para mergear
	vec2 position;
	vec2 scale;
	float rotation;
	float transparency;
	Texture* texture;

	// Guardo la matriz con la transformacion final para dibujar
	mat4 transformations;
	float transparencyToDraw;

	//Otros atributos
	bool destroyedAfterTransition;

	float interpolateParameter(float vIni, float vFin, float interval);
	vec2 interpolateParameter(vec2 vIni, vec2 vFin, float interval);
	UIEventHandler searchClickOnChilds(vec2 coords, int state);
	bool deleteTextureOnStateDelete;

public:
	UIState();
	virtual ~UIState(); 

	//**************************************************************************
	//*                            General methods		                       *
	//**************************************************************************
	//Clona todo el estado
	virtual UIState* clone();
	//Interpola los atributos del estado, con un estado final y guarda en estado destination
	void interpolate(UIState* finalState, UIState* destination, float interval);
	//Evalua si se realizo click en algun elemento de este estado
	UIEventHandler evaluateClick(vec2 coords, int state, UIState* parentState);
	
	void draw();
	void update(UIState* externalState);

	//Mergea las propiedades basicas(solo cosas que se interpolan) del estado actual con otro estado
	UIState mergeBasicParameters(UIState* state);
	//Idem pero sin mergear la escala
	UIState mergeBasicParametersWithoutParentScale(UIState* state);
	//Idem pero mergeando escala y corrigiendo la posicion del elemento para adaptarse a la escala del padre.
	UIState mergeBasicParametersWithParentScale(UIState* state);
		
	//**************************************************************************
	//*                                Accesors                                *
	//**************************************************************************
	void setPosition(vec2 position){ this->position = position;}
	vec2 getPosition(){ return this->position;}
	
	void setScale(vec2 scale){ this->scale = scale;}
	vec2 getScale(){ return this->scale;}

	void setRotation(float rotation){ this->rotation = rotation;}
	float getRotation(){ return this->rotation;}

	void setTransparency(float value){ this->transparency = value;}
	float getTransparency(){ return this->transparency;}

	// Cambia la textura pero sin borrar la anterior (se puede indicar si borrar o no cuando se borre el estado actual)
	void setTexture(Texture* texture, bool deleteTextureOnStateDelete=false);	

	// Cambia la textura y borra la textura anterior (se puede indicar si borrar o no cuando se borre el estado actual)
	void setTextureAndDeletePrevious(Texture* texture, bool deleteTextureOnStateDelete=false);

	Texture* getTexture(){ return this->texture;}

	list<UIElement*>* getDrawingElements(){ return &this->drawingElements; }
	void addElement(UIElement* element){this->drawingElements.push_back(element);}

	UIState* getReferenceToOriginal(){ return this->referenceToOriginal; }
	void setReferenceToOriginal(UIState* original){ this->referenceToOriginal = original; }

	//Identifica si el estado debe borrarse luego de producirse una transicion
	bool isDestroyedAfterTransition(){ return this->destroyedAfterTransition; }
	void setDestroyedAfterTransition(bool value){ this->destroyedAfterTransition = value; }
		
};