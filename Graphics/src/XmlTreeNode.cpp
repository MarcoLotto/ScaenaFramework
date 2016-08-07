/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "XmlTreeNode.h"

XmlTreeNode::XmlTreeNode(string name, XmlTreeNode* parent){
	this->name = name;
	this->parent = parent;
	this->value = "";
	this->binary = false;
}
XmlTreeNode::XmlTreeNode(string name, string value, XmlTreeNode* parent){
	this->name = name;
	this->parent = parent;
	this->value = value;
	this->binary = false;
}
void XmlTreeNode::setName(string name){
	this->name = name;
}
void XmlTreeNode::setValue(string value){
	this->value = value;
}
string XmlTreeNode::getName(){
	return this->name;
}
string XmlTreeNode::getValue(){
	return this->value;
}
XmlTreeNode* XmlTreeNode::getParent(){
	return this->parent;
}
void XmlTreeNode::addChild(XmlTreeNode* childNode){
	this->childNodes.push_back(childNode);
}
void XmlTreeNode::addAttribute(XmlNodeAttribute* attribute){
	this->attributes.push_back(attribute);
}
vector<XmlTreeNode*> XmlTreeNode::getChildNodes(){
	return this->childNodes;
}
void XmlTreeNode::setBinary(bool isBinary){
	this->binary = isBinary;
}
bool XmlTreeNode::isBinary(){
	return this->binary;
}
vector<XmlNodeAttribute*> XmlTreeNode::getAttributes(){
	return this->attributes;
}
void XmlTreeNode::createAttribute(string name, string value){
	XmlNodeAttribute* attribute = new XmlNodeAttribute();
	attribute->setName(name);
	attribute->setValue(value);
	this->addAttribute(attribute);
}

XmlTreeNode* XmlTreeNode::searchDirectChild(string childName, int iteration){
	int it = 0;
	for(int i=0; i<this->getChildNodes().size(); i++){
		XmlTreeNode* childNode = this->getChildNodes()[i];
		if(childNode->getName().compare(childName)==0){
			if(it == iteration)
				return childNode;
			it++;
		}
	}
	return NULL;
}

XmlNodeAttribute* XmlTreeNode::searchForAttribute(string attributeName){
	for(int i=0; i<this->getAttributes().size(); i++){
		XmlNodeAttribute* attribute = this->getAttributes()[i];
		if(attribute->getName().compare(attributeName)==0)
			return attribute;
	}
	return NULL;
}

XmlTreeNode::~XmlTreeNode(){
    // Borramos los atributos
    for(unsigned int i=0; i < this->getAttributes().size(); i++){
        delete this->getAttributes()[i];
    }
    this->getAttributes().clear();
    
    // Borramos todos los nodos hijos
    for(unsigned int i=0; i < this->getChildNodes().size(); i++){
        delete this->getChildNodes()[i];
    }
    this->getChildNodes().clear();
}

