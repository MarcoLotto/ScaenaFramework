/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include <glm/glm.hpp>
#include "XmlTreeNode.h"

typedef glm::vec3 vec3;
typedef glm::vec2 vec2;
typedef glm::mat4 mat4;

class XmlUtils{

private:
	static void serializeVec3ToNode(glm::vec3 vector, XmlTreeNode* parent);
	static void serializeVec2ToNode(glm::vec2 vector, XmlTreeNode* parent);
	static void floatEncoder(float value, unsigned char* packed);
	static float floatDecoder(unsigned char* packed);
	static void unsignedIntEncoder(unsigned int value, unsigned char* packed);
	static unsigned int unsignedIntDecoder(unsigned char* packed);

public:
	static void serializeToNode(string propertyName, glm::vec3 vector, XmlTreeNode* parent);
	static void serializeToNode(string propertyName, glm::vec2 vector, XmlTreeNode* parent);
	static void serializeToNode(string propertyName, float value, XmlTreeNode* parent);
	static void serializeToNode(string propertyName, string value, XmlTreeNode* parent);
	static vec3 getVec3FromNode(XmlTreeNode* node);
	static vec2 getVec2FromNode(XmlTreeNode* node);
	static mat4 getMatrix4x4FromBuffer(float* buffer);

	// Permiten parsear un buffer de datos parseandolos a formato binario para que tenga el menor peso posible y mayor velocidad de desempaquetado
	static void parseBufferInBinaryToNode(string bufferTagName, unsigned int* bufferData, int bufferSize, XmlTreeNode* parentNode);
	static void parseBufferInBinaryToNode(string bufferTagName, float* bufferData, int bufferSize, XmlTreeNode* parentNode);

	// Dado un nodo padre, se le indica el nombre de un nodo hijo cuyo contenido es un buffer binario. 
	// Devuelve un buffer del tipo indicado con la data y el tamaño del mismo en el puntero a size. 
	static float* createFloatBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size);
	static unsigned int* createUnsignedIntBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size);
	static unsigned short* createUnsignedShortBufferFromBinaryNode(string dataNodeName, XmlTreeNode* parentNode, int* size);
};
