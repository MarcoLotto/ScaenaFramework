/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

// Identifica un punto de toque con pantalla touch o panel tactil
class TouchPoint{

private:	
	float x;
	float y;
	int id;
	bool changed;

public:
	TouchPoint(float positionX, float positionY, int touchId, bool hasChanged){ 		
		this->x = positionX;
		this->y = positionY;
		this->id = touchId;
		this->changed = hasChanged;		
	}

	// Informaci{on del punto, como su posicion, su id, y si es el punto que cambio en este evento
	float getX(){ return this->x; }
	float getY(){ return this->y; }

	int getId(){ return this->id; }
	bool hasChanged(){ return this->changed; }

	void setX(float x){ this->x = x; }
	void setY(float y){ this->y = y; }
};