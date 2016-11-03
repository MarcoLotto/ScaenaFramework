/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "XmlUtils.h"
#include "StringUtils.h"
#include "DataConformationException.h"

void XmlUtils::serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);
}

void XmlUtils::serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode("X", StringUtils::toString(vector.x), parent);
	parent->addChild(xNode);
	
	XmlTreeNode* yNode = new XmlTreeNode("Y", StringUtils::toString(vector.y), parent);
	parent->addChild(yNode);

	XmlTreeNode* zNode = new XmlTreeNode("Z", StringUtils::toString(vector.z), parent);
	parent->addChild(zNode);
}

void XmlUtils::serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	serializeVec3ToNode(vector, propertyNode);
}

void XmlUtils::serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent){
	XmlTreeNode* propertyNode = new XmlTreeNode(propertyName, parent);
	parent->addChild(propertyNode);
	serializeVec2ToNode(vector, propertyNode);
}

void XmlUtils::serializeToNode(string propertyName, float value, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode(propertyName, StringUtils::toString(value), parent);
	parent->addChild(xNode);
}

void XmlUtils::serializeToNode(string propertyName, string value, XmlTreeNode* parent){
	XmlTreeNode* xNode = new XmlTreeNode(propertyName, value, parent);
	parent->addChild(xNode);
}

vec3 XmlUtils::getVec3FromNode(XmlTreeNode* node){
	vec3 vec = glm::vec3(0.0f);
	//X
	XmlTreeNode* childNode = node->searchDirectChild("X", 0);
	if(childNode)
		vec.x = atof(childNode->getValue().c_str());
	//Y
	childNode = node->searchDirectChild("Y", 0);
	if(childNode)
		vec.y = atof(childNode->getValue().c_str());
	//Z
	childNode = node->searchDirectChild("Z", 0);
	if(childNode)
		vec.z = atof(childNode->getValue().c_str());
	return vec;
}

vec2 XmlUtils::getVec2FromNode(XmlTreeNode* node){
	vec2 vec = vec2(0.0f);
	//X
	XmlTreeNode* childNode = node->searchDirectChild("X", 0);
	if(childNode)
		vec.x = atof(childNode->getValue().c_str());
	//Y
	childNode = node->searchDirectChild("Y", 0);
	if(childNode)
		vec.y = atof(childNode->getValue().c_str());
	return vec;
}

mat4 XmlUtils::getMatrix4x4FromBuffer(float* buffer){
	mat4 matrix;
	for(unsigned int i=0; i < 4; i++){
		for(unsigned int j=0; j < 4; j++){
			matrix[i][j] = buffer[i*4+j];
		}
	}
	return matrix;
}

void XmlUtils::parseBufferInBinaryToNode(string bufferTagName, float* bufferData, int bufferSize, XmlTreeNode* parentNode){
	XmlTreeNode* bufferNode = new XmlTreeNode(bufferTagName, parentNode);
	parentNode->addChild(bufferNode);

	// Agrego como atributo el largo del buffer y su tipo
	bufferNode->addAttribute(new XmlNodeAttribute("Size", StringUtils::toString(bufferSize)));
	bufferNode->addAttribute(new XmlNodeAttribute("Type", "float"));

	// Parseo la data del buffer y la agrego
	string value = "";
	unsigned char auxBuffer[4];	
	for(int i=0; i < bufferSize ; i++){
		floatEncoder(bufferData[i], auxBuffer);
		// El string guarda chars, no unsigned chars, por lo que le resto 127 a cada unsigned char
		value += auxBuffer[0]-127;  value += auxBuffer[1]-127; value += auxBuffer[2]-127; value += auxBuffer[3]-127;		
	}
	bufferNode->setValue(value);
	bufferNode->setBinary(true);
}

void XmlUtils::parseBufferInBinaryToNode(string bufferTagName, unsigned int* bufferData, int bufferSize, XmlTreeNode* parentNode){
	XmlTreeNode* bufferNode = new XmlTreeNode(bufferTagName, parentNode);
	parentNode->addChild(bufferNode);

	// Agrego como atributo el largo del buffer y su tipo
	bufferNode->addAttribute(new XmlNodeAttribute("Size", StringUtils::toString(bufferSize)));
	bufferNode->addAttribute(new XmlNodeAttribute("Type", "uint"));

	// Parseo la data del buffer y la agrego
	string value = "";
	unsigned char auxBuffer[4];	
	for(int i=0; i < bufferSize ; i++){
		unsignedIntEncoder(bufferData[i], auxBuffer);
		// El string guarda chars, no unsigned chars, por lo que le resto 127 a cada unsigned char
		value += auxBuffer[0]-127;  value += auxBuffer[1]-127; value += auxBuffer[2]-127; value += auxBuffer[3]-127;		
	}
	bufferNode->setValue(value);
	bufferNode->setBinary(true);
}

void XmlUtils::floatEncoder(float value, unsigned char* packed){
	unsigned int p;
    unsigned int sign;	

    if (value < 0) { sign = 1; value = -value; }
    else { sign = 0; }
        
    p = ((((unsigned int)value)&0x7fff)<<16) | (sign<<31); // whole part and sign
    p |= (unsigned int)(((value - (int)value) * 65536.0f))&0xffff; // fraction

	// Lo empaqueto en 4 bytes
	unsignedIntEncoder(p, packed);  
}

void XmlUtils::unsignedIntEncoder(unsigned int value, unsigned char* packed){
	// Empaqueto el valor
	packed[0] = value >> 24;
	packed[1] = value >> 16;
	packed[2] = value >> 8;
	packed[3] = value;
}

float* XmlUtils::createFloatBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size){
	XmlTreeNode* bufferNode = parentNode->searchDirectChild(dataNodeName, 0);
	if(bufferNode == NULL) return NULL;

	// Creo la estructura del tipo y tamaño correcto
	XmlNodeAttribute* sizeAtt = bufferNode->searchForAttribute("Size");
	if(sizeAtt == NULL) return NULL;
		
	int bufferSize = atoi(sizeAtt->getValue().c_str());
	*size = bufferSize;
	float* buffer = new float[bufferSize]();
	string fullValue = bufferNode->getValue();

	// Valido el tamaño del buffer
	if(fullValue.size()/4 != bufferSize){
		throw new DataConformationException("El tamaño real del buffer de tipo float, difiere del tamaño declado en el nodo (Tip: codifico su archivo en ASCII?)");
	}		
	// Guardo los datos del buffer en la nueva estructura	
	int separatorCount = 0;	
	for(unsigned int i = 0; i < fullValue.size() ; i+=4){	
		unsigned char auxBuffer[4];		
		auxBuffer[0]=fullValue.at(i)+127; auxBuffer[1]=fullValue.at(i+1)+127; auxBuffer[2]=fullValue.at(i+2)+127; auxBuffer[3]=fullValue.at(i+3)+127;
		buffer[separatorCount] = floatDecoder(auxBuffer); 		
		separatorCount++;						
	}	
	return buffer;
}

unsigned int* XmlUtils::createUnsignedIntBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size){
	XmlTreeNode* bufferNode = parentNode->searchDirectChild(dataNodeName, 0);
	if(bufferNode == NULL) return NULL;

	// Creo la estructura del tipo y tamaño correcto
	XmlNodeAttribute* sizeAtt = bufferNode->searchForAttribute("Size");
	if(sizeAtt == NULL) return NULL;
		
	int bufferSize = atoi(sizeAtt->getValue().c_str());
	*size = bufferSize;
	unsigned int* buffer = new unsigned int[bufferSize]();
	string fullValue = bufferNode->getValue();

	// Valido el tamaño del buffer
	if(bufferSize != fullValue.size()/4){
		throw new DataConformationException("El tamaño real del buffer de tipo unsigned int, difiere del tamaño declado en el nodo (Tip: codifico su archivo en ASCII?)");
	}		
	// Guardo los datos del buffer en la nueva estructura
	string auxValue = "";	
	int separatorCount = 0;
	for(unsigned int i = 0; i < fullValue.size() ; i+=4){		
		unsigned char auxBuffer[4];		
		auxBuffer[0]=fullValue.at(i)+127; auxBuffer[1]=fullValue.at(i+1)+127; auxBuffer[2]=fullValue.at(i+2)+127; auxBuffer[3]=fullValue.at(i+3)+127;
		buffer[separatorCount] = unsignedIntDecoder(auxBuffer); 		
		separatorCount++;						
	}
	return buffer;
}

unsigned short* XmlUtils::createUnsignedShortBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size){
    XmlTreeNode* bufferNode = parentNode->searchDirectChild(dataNodeName, 0);
    if(bufferNode == NULL) return NULL;
    
    // Creo la estructura del tipo y tamaño correcto
    XmlNodeAttribute* sizeAtt = bufferNode->searchForAttribute("Size");
    if(sizeAtt == NULL) return NULL;
    
    int bufferSize = atoi(sizeAtt->getValue().c_str());
    *size = bufferSize;
    unsigned short* buffer = new unsigned short[bufferSize]();
    string fullValue = bufferNode->getValue();
    
    // Valido el tamaño del buffer
    if(bufferSize != fullValue.size()/4){
        throw new DataConformationException("El tamaño real del buffer de tipo unsigned int, difiere del tamaño declado en el nodo (Tip: codifico su archivo en ASCII?)");
    }
    // Guardo los datos del buffer en la nueva estructura
    string auxValue = "";
    int separatorCount = 0;
    for(unsigned int i = 0; i < fullValue.size() ; i+=4){
        unsigned char auxBuffer[4];
        auxBuffer[0]=fullValue.at(i)+127; auxBuffer[1]=fullValue.at(i+1)+127; auxBuffer[2]=fullValue.at(i+2)+127; auxBuffer[3]=fullValue.at(i+3)+127;
        buffer[separatorCount] = (unsigned short) unsignedIntDecoder(auxBuffer);
        separatorCount++;						
    }
    return buffer;
}

unsigned int XmlUtils::unsignedIntDecoder(unsigned char* packed){	
	return ((((((packed[0] << 8) | packed[1]) << 8) | packed[2]) << 8) | packed[3]);
}

float XmlUtils::floatDecoder(unsigned char* packed){
	unsigned int unpackedValue = unsignedIntDecoder(packed);
	float decodedValue = ((unpackedValue>>16)&0x7fff); // whole part
    decodedValue += (unpackedValue&0xffff) / 65536.0f; // fraction
    if (((unpackedValue>>31)&0x1) == 0x1) { decodedValue = -decodedValue; }  // sign bit set   
    return decodedValue;
}