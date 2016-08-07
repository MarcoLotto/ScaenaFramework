/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "XmlTree.h"

XmlTree::XmlTree(){
	this->rootNode = new XmlTreeNode("root", NULL);
	this->lastNodeVisited = this->rootNode;
}

XmlTreeNode* XmlTree::getLastNodeVisited(){
	return this->lastNodeVisited;
}
void XmlTree::setLastNodeVisited(XmlTreeNode* node){
	this->lastNodeVisited = node;
}

XmlTreeNode* XmlTree::getRootNode(){
	return this->rootNode;
}

XmlTree::~XmlTree(){
    delete this->rootNode;
}