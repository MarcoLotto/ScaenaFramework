/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

// Interfaz comun para aquellos objetos que cargan vertices (u otra informacion relacionada) en memoria de video
class DataBuffer{

protected:
	unsigned int handler;
	virtual void initiateInMemory() = 0;
	bool alreadyLoaded;

public:
	DataBuffer(){ this->alreadyLoaded = false; }

	bool isAlreadyLoaded(){ return this->alreadyLoaded; }
	void setAlreadyLoaded(bool value){ this->alreadyLoaded = value; }

	unsigned int getHandler(){ return this->handler; }
	void setHandler(unsigned int handler){ this->handler = handler; }
};