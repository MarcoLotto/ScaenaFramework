/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include <string>
#include <iostream>
#include <sstream>
#include "XmlParser.h"
#include "XmlNodeAttribute.h"
#include <iostream>
#include <fstream>
#include "StringUtils.h"
#include "FileProcessingException.h"
#include <string.h>
using namespace std;

#define BINARY_START_BLOCK "@B["
#define BINARY_END_BLOCK "]@B"

XmlTree* XmlParser::parse(const char* filename){		
	XmlTree* xmlTree = new XmlTree();
	vector<string>* attributes = new vector<string>(); 
	
	FILE *fp;
	fp = fopen ( filename , "rb" );	
	if (fp) {		
				
		// Leo todo el archivo y me lo guardo en un char*
		long lSize;
		fseek( fp , 0L , SEEK_END);
		lSize = ftell( fp );
		rewind( fp );
				
		char *bufferData = new char[lSize]();
		if( !bufferData ){
			fclose(fp);
			throw new FileProcessingException("Fatal XmlParselError in \"%s\, no memory avaible\n" + string(filename));
		}
		// Copia el archivo en el buffer
		if( 1!=fread( bufferData , lSize, 1 , fp) ){
			fclose(fp);
			throw new FileProcessingException("Fatal XmlParselError in \"%s\, read error\n" + string(filename));
		}		
		fclose(fp);		
			
		// Voy a procesar el archivo cargado, mientras quede algo que procesar
		unsigned int positionInData = 0;
		while(positionInData < lSize > 0){
			if(!parseXmlNode(bufferData, attributes, xmlTree, (int*)&positionInData, lSize)){
				throw new FileProcessingException("Fatal XmlParselError in \"%s\, Xml parse error\n" + string(filename));
				delete xmlTree;
				delete attributes;
				delete bufferData;
				return NULL;
			}
		}	
		delete bufferData;
	}	
	delete attributes;	
	return xmlTree;
}

bool processNodeEnd(const char* data, int* positionInData, unsigned int dataSize, char* nodeValue, unsigned int nodeValueSize, XmlTree* xmlTree, bool isBinary){
	XmlTreeNode* node = xmlTree->getLastNodeVisited();
	node->setBinary(isBinary);
	node->setValue(string(nodeValue, nodeValueSize));
	delete nodeValue;
		
	//Busco el '/' y avanzo hasta esa posicion
	int closeTagPosition = StringUtils::find(data, *positionInData, dataSize, '/');
	if(closeTagPosition == -1){
		return true;  //No hay cierre, podria ser que se este abriendo otro tag
	}
	*positionInData += closeTagPosition + 1;
	if(*positionInData >= dataSize){
		return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
	}
	
	//Me guardo el nombre del tag que cierra
	int endTagPosition = StringUtils::find(data, *positionInData, dataSize, '>');
	if(endTagPosition == -1){
		return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
	}	
	char* closingName = new char[endTagPosition]();	
	StringUtils::substring(closingName, data, *positionInData, endTagPosition);

	//El valor de cierre de nodo y el nombre del nodo deben ser iguales
	string closeFinalName = string(closingName).substr(0, endTagPosition);
	if(closeFinalName.compare(node->getName()) != 0){
		delete closingName;
		return false;
	}
	delete closingName;

	//Subo en la gerarquia de nodos
	xmlTree->setLastNodeVisited(node->getParent());

	//Borro el tag de cierre y termine con el nodo
	*positionInData += endTagPosition + 1;
	return true;
}

bool XmlParser::parseXmlNode(const char* data, vector<string>* attributes, XmlTree* xmlTree, int* positionInData, unsigned int dataSize){
	bool processContent = true;

	//Consigo la posicion del proximo tag de <
	int startTagPosition = StringUtils::find(data, *positionInData, dataSize, '<');
	if(startTagPosition == -1){
		*positionInData = dataSize;
		return true;  //No hay mas declaraciones, borro y sigo
	}

	//Consigo el posible valor del nodo
	char* nodeValue = new char[startTagPosition]();	
	StringUtils::substring(nodeValue, data, *positionInData, startTagPosition);

	// Actualizo la posicion en donde leo la data hasta despues del '<'	
	*positionInData += startTagPosition + 1;
	if(*positionInData >= dataSize){
		return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
	}
	
	// Controlo que simbolo viene despues del '<' y actuo segun corresponda
	if(data[*positionInData] == '?')
		processContent=false; //Es una declaracion de XML, la ignoro
	if((dataSize - *positionInData >= 3)&&(data[*positionInData]=='!' && data[*positionInData+1]=='-' && data[*positionInData+2]=='-'))
		processContent=false; //Es un comentario, lo ignoro
	if(data[*positionInData]=='/'){ //Es un cierre de nodo, guardo el valor y termino con el mismo
		return processNodeEnd(data, positionInData, dataSize, nodeValue, startTagPosition, xmlTree, false);
	}
	delete nodeValue;

	//Busco el '>'
	int endTagPosition = StringUtils::find(data, *positionInData, dataSize, '>');
	if(endTagPosition == -1){
		*positionInData = dataSize;
		return false;  //No hay cierre, error
	}

	//Consigo el contenido entre los tags '<' y '>'
	char* content = new char[endTagPosition]();	
	StringUtils::substring(content, data, *positionInData, endTagPosition);

	// Actualizo la posicion en donde leo la data hasta despues del '>'	
	*positionInData += endTagPosition + 1;
	if(*positionInData >= dataSize){
		delete content;
		return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
	}
	
	//Si no tengo que ignorarlo, proceso el contenido
	if(processContent){
		findAttributes(content, endTagPosition, attributes, xmlTree);
	}
	delete content;  

	// Verifico si es un campo binario, de ser asi me guardo el valor
	int blockStartPos = StringUtils::find(data, *positionInData, dataSize, BINARY_START_BLOCK);
	startTagPosition = StringUtils::find(data, *positionInData, dataSize, '<');
	if(blockStartPos != -1 && blockStartPos < startTagPosition){
		
		// Actualizo la posicion en donde leo la data hasta despues de la declaracion de bloque binario	
		*positionInData += blockStartPos + strlen(BINARY_START_BLOCK);
		if(*positionInData >= dataSize){
			return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
		}

		int blockEndPos = StringUtils::find(data, *positionInData, dataSize, BINARY_END_BLOCK);			
		if(blockEndPos != -1){
			char* binaryContent = new char[blockEndPos]();	
			StringUtils::substring(binaryContent, data, *positionInData, blockEndPos);

			// Actualizo la posicion en donde leo la data hasta despues de cerrado el bloque binario
			*positionInData += blockEndPos + strlen(BINARY_END_BLOCK);
			if(*positionInData >= dataSize){
				delete binaryContent;
				return false;  // Termine de leer todo el texto pero me quedo un '<' sin cerrar
			}

			// Busco el siguiente '<' y avanzo hasta alli
			endTagPosition = StringUtils::find(data, *positionInData, dataSize, '<');
			*positionInData += endTagPosition + 1;
			if(endTagPosition == -1 || *positionInData >= dataSize){
				delete binaryContent;
				return false;  // No encontre el tag de cierre
			}
			
			// Mando a guardar el content en el nodo
			return processNodeEnd(data, positionInData, dataSize, binaryContent, blockEndPos, xmlTree, true);
		}
	}
	return true;
}

void XmlParser::findAttributes(char* content, int contentSize, vector<string>* attributes, XmlTree* xmlTree){
	//Voy buscando espacios y guardando los atributos
	int lastSpacePosition = 0;
	bool commentArea = false;
	for(int i=0; i < contentSize; i++){
		if(content[i] == '"'){  // Verifico que no me encuentre entre " "
			commentArea = !commentArea;
		}
		if(((content[i] == ' ') || (i == contentSize - 1)) && !commentArea){	// Si tengo un espacio y no estoy entre " ", tengo un atributo			
			if(content[lastSpacePosition] == ' '){
				lastSpacePosition++;  // Remuevo el espacio
			}
			if(content[i] != ' '){
				i++; // Estoy en el caso del ultimo caracter de la linea, si no es un espacio quiero tenerlo en cuenta
			}
			if(lastSpacePosition < i){
				char* auxAttribute = new char[i - lastSpacePosition]();
				StringUtils::substring(auxAttribute, content, lastSpacePosition, i - lastSpacePosition);			
				string attribute = string(auxAttribute);			
				attributes->push_back(attribute.substr(0, i - lastSpacePosition));
				delete auxAttribute;
			}		
			lastSpacePosition = i;
		}
	}
	parseAttributes(attributes, xmlTree);
	attributes->clear();
}

XmlNodeAttribute* parseParticularAttribute(string attribute){
	XmlNodeAttribute* nodeAttribute = new XmlNodeAttribute();
	string name = attribute.substr(0,attribute.find("=\""));
	string value = attribute.erase(0,attribute.find("=\"")+2);
	value = value.substr(0,value.find("\""));
	nodeAttribute->setName(name);
	nodeAttribute->setValue(value);
	return nodeAttribute;
}


void XmlParser::parseAttributes(vector<string>* attributes, XmlTree* xmlTree){
	for(unsigned int i=0; i < attributes->size(); i++){
		string attribute = attributes->at(i);
		XmlTreeNode* node = xmlTree->getLastNodeVisited();
		if(i==0){  //Es el nombre del nodo
			XmlTreeNode* childNode = new XmlTreeNode(attribute, node);
			node->addChild(childNode);  //Creo un nodo hijo y le seteo el nombre
			xmlTree->setLastNodeVisited(childNode);
		}
		else{
			//Le seteo el atributo al nodo
			node->addAttribute(parseParticularAttribute(attribute));
		}
	}
}

string printTabs(int tabs){
	string finalTabs = "";
	for(int i=0; i<tabs; i++){
		finalTabs += "\t";
	}
	return finalTabs;
}

string printNodeAttributes(XmlTreeNode* childNode){
	string finalAtt = "";
	for(unsigned int i=0; i < childNode->getAttributes().size(); i++){
		XmlNodeAttribute* attribute = childNode->getAttributes().at(i);
		finalAtt += " " + attribute->getName() + "=\"" + attribute->getValue() + "\"";
	}
	return finalAtt;
}

bool writeNode(ofstream *myfile, XmlTreeNode* node, int tabs){
	bool haveChilds = false;
	for(unsigned int i=0; i < node->getChildNodes().size(); i++){		
		XmlTreeNode* childNode = node->getChildNodes().at(i);

		// Si es un binario, primero seteo los bloques que identifican binario
		string finalValueToWrite = childNode->getValue();
		if(childNode->isBinary()){
			finalValueToWrite = BINARY_START_BLOCK + finalValueToWrite + BINARY_END_BLOCK;
		}
		
		*myfile << "\n" + printTabs(tabs) + "<" + childNode->getName() + printNodeAttributes(childNode) + ">" + finalValueToWrite;
		
		string endLine = "";
		if(writeNode(myfile, childNode, tabs+1))
			endLine = printTabs(tabs);

		*myfile << endLine + "</" + childNode->getName() + ">" + "\n";
		haveChilds = true;
	}
	return haveChilds;
}

 void XmlParser::writeXmlToFile(const char* filename, XmlTree* xmlTree){
	ofstream myfile;
	myfile.open (filename, ios::binary);
	if(myfile){
		writeNode(&myfile, xmlTree->getRootNode(), 0);
	}
	else{
		printf("Error en escritura Xml del archivo %s", filename);
		return;
	}
	myfile.close();
 }
