/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <string>
using namespace std;

class XmlNodeAttribute{

private:
	string name;
	string value;
	
public:
	XmlNodeAttribute();
	XmlNodeAttribute(string name, string value);
	XmlNodeAttribute(string name, float value);

	void setName(string name);
	void setValue(string value);
	string getName();
	string getValue();
};