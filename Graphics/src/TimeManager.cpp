/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "TimeManager.h"
#include "WindowConnector.h"
#include <stdlib.h>

TimeManager* TimeManager::instance = NULL;

TimeManager* TimeManager::getInstance(){
	if(instance == NULL){
		instance = new TimeManager();	
		instance->emptyTimeSlots = 0;
	}
	return instance;
}

TimeManager::TimeManager(){
    this->onPauseOSTime = 0;
    this->isSystemPaused = false;
} 

unsigned int TimeManager::reservTimeSlot(){
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
	
void TimeManager::updateTimeSlot(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size())
		this->timeSlots[timeSlot] = WindowConnector::GetOSCurrentTime();	
}
	
int TimeManager::getElapsedTime(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size())
		return WindowConnector::GetOSCurrentTime() - this->timeSlots[timeSlot];
	return 0;
}

void TimeManager::removeTimeSlot(unsigned int timeSlot){
	if(timeSlot < this->timeSlots.size()){
		this->timeSlots[timeSlot] = -1;  //No lo borro, lo invalido
		this->emptyTimeSlots++; //Aumento la cant de timeSlots sin uso
	}
}

int TimeManager::getOSTimeInMilliseconds(){
	return WindowConnector::GetOSCurrentTime();
}

void TimeManager::pause(){
    if(!this->isSystemPaused){
        this->isSystemPaused = true;
        this->onPauseOSTime = this->getOSTimeInMilliseconds();
    }
}

void TimeManager::resume(){
    if(this->isSystemPaused){
        unsigned int totalTimeOnPause = this->getOSTimeInMilliseconds() - this->onPauseOSTime;
        for(unsigned int i=0; i < this->timeSlots.size(); i++){
            if(this->timeSlots[i] != -1){
                this->timeSlots[i] = this->timeSlots[i] + totalTimeOnPause;
            }
        }
        this->isSystemPaused = false;
    }
}

