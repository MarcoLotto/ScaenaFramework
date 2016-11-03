/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <vector>
using namespace std;

//********************************UnstopableTimeManager*****************************************
//* Manaja el tiempo del sistema. Funciona del sig modo, se reserva un time slot, luego
//* se le pregunta al mismo el tiempo trancurrido. El time slot puede actualizarse.
//* Tiene como diferencia con el TimeManager, que este no se detiene al pausar el programa,
//* los contadores siguen corriendo
//**********************************************************************************************
class UnstopableTimeManager{

private:
	vector<int> timeSlots;
	int emptyTimeSlots;
    
	static UnstopableTimeManager* instance;
    UnstopableTimeManager();
    
public:
	static UnstopableTimeManager* getInstance();
	static int getOSTimeInMilliseconds();

	//Reserva un time slot con el tiempo actual, devuelve el identificador del time slot
	unsigned int reservTimeSlot();
	//Actualiza el time slot al tiempo actual
	void updateTimeSlot(unsigned int timeSlot);
	//Devuelve el tiempo transcurrido en milisegundos desde la ultima actualizacion del time slot
	int getElapsedTime(unsigned int timeSlot);
	//Borra un timeSlot
	void removeTimeSlot(unsigned int timeSlot);
};