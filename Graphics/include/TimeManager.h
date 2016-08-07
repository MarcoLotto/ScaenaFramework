/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

#include <vector>
using namespace std;

//********************************TimeManager*****************************************
//* Manaja el tiempo del sistema. Funciona del sig modo, se reserva un time slot, luego
//* se le pregunta al mismo el tiempo trancurrido. El time slot puede actualizarse.
//************************************************************************************
class TimeManager{

private:
	vector<int> timeSlots;
	int emptyTimeSlots;
    unsigned int onPauseOSTime;
    bool isSystemPaused;
     
	static TimeManager* instance;
    TimeManager();
    
public:
	static TimeManager* getInstance();
	static int getOSTimeInMilliseconds();

	//Reserva un time slot con el tiempo actual, devuelve el identificador del time slot
	unsigned int reservTimeSlot();
	//Actualiza el time slot al tiempo actual
	void updateTimeSlot(unsigned int timeSlot);
	//Devuelve el tiempo transcurrido en milisegundos desde la ultima actualizacion del time slot
	int getElapsedTime(unsigned int timeSlot);
	//Borra un timeSlot
	void removeTimeSlot(unsigned int timeSlot);
    
    // Pausan y resumen todos los tiempos del sistema
    void pause();
    void resume();
};
