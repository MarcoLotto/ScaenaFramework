/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "TouchPoint.h"
#include "SensorsData.h"
#include <vector>
using namespace std;

enum TouchType { began, moved, ended, cancelled, invalid };

// Identifica un evento de toque (es decir todos los toques actuales y cambios de estado)
class TouchEvent{

private:
	TouchType type;  // El tipo de evento que se produjo (movio, iniciado, etc)		
	int touchTime;	
	SensorsData sensorsData;
	
	vector<TouchPoint*>* touchPoints;

public:
	TouchEvent(TouchType type, int touchTime){ 
		this->type = type; 				
		this->touchTime = touchTime;		
		this->touchPoints = new vector<TouchPoint*>();
	}

	~TouchEvent(){ 
		vector<TouchPoint*>::iterator it = this->touchPoints->begin();
		while(it != this->touchPoints->end()){
			delete *it;
			it = this->touchPoints->erase(it);
		}		
		delete this->touchPoints;
	}

	// Agrega un punto de toque simultaneo a los demás
	void addTouchPoint(TouchPoint* point){ this->touchPoints->push_back(point); }
	vector<TouchPoint*>* getTouchPoints(){ return this->touchPoints; }
	
	TouchType getTouchType(){ return this->type; }	
	int getTouchTime(){ return this->touchTime; }
	int getTouchCount(){ return this->touchPoints->size(); }

	// Devuelve la estructura para declarar eventos de sonsores
	SensorsData& getSensorsData(){ return this->sensorsData; }
};
