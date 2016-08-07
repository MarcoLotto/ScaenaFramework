/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <string>
using namespace std;

#define INFO_LOG_TYPE 0
#define DEBUG_LOG_TYPE 1
#define ERROR_LOG_TYPE 2

class Log{

private:
	string message;
	int type;

public:
	Log(string message, int type=0){
		this->message = message;
		this->type = type;
	}

	string getMessage(){ return this->message; }
	int getType(){ return this->type; }
	void setType(int type){ this->type = type;}
};