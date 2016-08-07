/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlNodeAttribute.h"
#include <vector>
using namespace std;

class XmlTreeNode{

private:
	string name;
	string value;
	bool binary;
	XmlTreeNode* parent;
	vector<XmlTreeNode*> childNodes;
	vector<XmlNodeAttribute*> attributes;
public:
	XmlTreeNode(string name, XmlTreeNode* parent);
	XmlTreeNode(string name, string value, XmlTreeNode* parent);
    virtual ~XmlTreeNode();
    
	void setName(string name);
	void setValue(string value);
	string getName();
	string getValue();
	void setBinary(bool isBinary);
	bool isBinary();
	XmlTreeNode* getParent();
	void addChild(XmlTreeNode* childNode);
	void addAttribute(XmlNodeAttribute* attribute);
	vector<XmlTreeNode*> getChildNodes();
	vector<XmlNodeAttribute*> getAttributes();
	void createAttribute(string name, string value);
	XmlTreeNode* searchDirectChild(string childName, int iteration);
	XmlNodeAttribute* searchForAttribute(string attributeName);
};