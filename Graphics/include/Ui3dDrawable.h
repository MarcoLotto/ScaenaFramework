/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "UIElement.h"
#include "ForwardGeometryStage.h"
#include "Scene.h"
#include <string>
using namespace std;

class Ui3dDrawable : public UIElement{

protected:
	//Maneja los eventos que se producen cuando un elemento consigue el foco
	void handleEvents();

private:
	//Incializacion de los estados del elemento 
	void initiateStates();
	//Inicializacion de los subelementos
	void initiateElements();
	//Linkea los elementos con los estados (cuales deben ser dibujados en cada estado)
	void bindElementsToStates();
	
	//Estados	
	UIState* basicState; 
	
	// Voy a necesitar dibujar una escena 3d, vamos a utilizar forward shading. Uso un stage.
	ForwardGeometryStage* geometryStage;
	Scene* sceneToDraw;


	// Actualiza y dibuja la escena en el framebuffer para ser utilizada posteriormente
	void drawScene();
	
	// Consigue un framebuffer que se adapte a lo necesitado y lo bindea
	void manageAndBindFrameBuffer();	

	// Calcula el tamaño del area de renderizado
	vec2 getDrawableAreaSize();
		
public:
	Ui3dDrawable(Scene* scene, UIController* uiController);
	virtual ~Ui3dDrawable();

	void draw();
	void update(UIState* externalState);

	//Accesors
	void setSceneToDraw(Scene* scene);
	Scene* getSceneToDraw();
};