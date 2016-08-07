/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once

// Interfaz generica para aquellos elementos que soporten llamados de callback
class Callbackable{

public:
	virtual void onCallback(int callbackId) = 0;
};