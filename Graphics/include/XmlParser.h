/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "XmlTree.h"

class XmlParser{

private:
	static void parseAttributes(vector<string>* attributes, XmlTree* xmlTree);
	static void findAttributes(char *content, int contentSize, vector<string>* attributes, XmlTree* xmlTree);
	static bool parseXmlNode(const char* data, vector<string>* attributes, XmlTree* xmlTree, int* positionInData, unsigned int dataSize);
public:
	static XmlTree* parse(const char* filename);
	static void writeXmlToFile(const char* filename, XmlTree* xmlTree);
};
