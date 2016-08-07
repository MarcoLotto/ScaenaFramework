/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "XmlNodeAttribute.h"
#include "StringUtils.h"

XmlNodeAttribute::XmlNodeAttribute(){
	this->name = "";
	this->value = "";
}
XmlNodeAttribute::XmlNodeAttribute(string name, string value){
	this->name = name;
	this->value = value;
}
XmlNodeAttribute::XmlNodeAttribute(string name, float value){
	this->name = name;
	this->value = StringUtils::toString(value);
}
void XmlNodeAttribute::setName(string name){
	this->name = name;
}
void XmlNodeAttribute::setValue(string value){
	this->value = value;
}
string XmlNodeAttribute::getName(){
	return this->name;
}
string XmlNodeAttribute::getValue(){
	return this->value;
}
