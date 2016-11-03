/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlTreeNode.h"

class XmlTree{

private:
	XmlTreeNode* rootNode;
	XmlTreeNode* lastNodeVisited;
public:
	XmlTree();
    virtual ~XmlTree();
	XmlTreeNode* getLastNodeVisited();
	void setLastNodeVisited(XmlTreeNode* node);
	XmlTreeNode* getRootNode();
};