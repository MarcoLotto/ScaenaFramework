/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "AnimationDataTransfer.h"
#include "XmlTreeNode.h"
#include <string>
using namespace std;

class GenerationProcessor{

private:
	static XmlTreeNode* searchForAnimation(XmlTreeNode* safNode, string animationName);

public:
	static bool processScaenaModelFile(const char* filename);
	static bool GenerationProcessor::processScaenaAnimationFile(string& inAnimationFile, string& outAnimationFile, string& keyFrameFBXFile, string& animationName, float keyDuration);
};
