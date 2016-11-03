/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "UnstopableTimeManager.h"
#include "WindowConnector.h"
#include <stdlib.h>

UnstopableTimeManager* UnstopableTimeManager::instance = NULL;

UnstopableTimeManager* UnstopableTimeManager::getInstance(){
	if(instance == NULL){
		instance = new UnstopableTimeManager();	
		instance->emptyTimeSlots = 0;
	}
	return instance;
}

UnstopableTimeManager::UnstopableTimeManager(){
}

unsigned int UnstopableTimeManager::reservTimeSlot(){
	int actualTime = WindowConnector::GetOSCurrentTime();
	//Primero verifico si hay timeSlots sin uso
	if(this->emptyTimeSlots > 0){
		for(unsigned int i=0; i < this->timeSlots.size(); i++){
			if(this->timeSlots[i] == -1){
				this->timeSlots[i] = actualTime;
				this->emptyTimeSlots--;
				return i;
			}
		}
	}
	//Si no hay timeSlots sin uso, creo uno nuevo
	this->timeSlots.push_back(actualTime);
	return this->timeSlots.size() - 1;
}
	
void UnstopableTimeManager::updateTimeSlot(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size())
		this->timeSlots[timeSlot] = WindowConnector::GetOSCurrentTime();	
}
	
int UnstopableTimeManager::getElapsedTime(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size())
		return WindowConnector::GetOSCurrentTime() - this->timeSlots[timeSlot];
	return 0;
}

void UnstopableTimeManager::removeTimeSlot(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size()){
		this->timeSlots[timeSlot] = -1;  //No lo borro, lo invalido
		this->emptyTimeSlots++; //Aumento la cant de timeSlots sin uso
	}
}

int UnstopableTimeManager::getOSTimeInMilliseconds(){
	return WindowConnector::GetOSCurrentTime();
}


