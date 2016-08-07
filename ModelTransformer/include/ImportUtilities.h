/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SkeletonDataTransfer.h"

class ImportUtilities{

private:
	static SkeletonDataTransfer* internalFindJointSkelletonUsingName(const std::string& inJointName, SkeletonDataTransfer* skeletonDataTransfer);
	static SkeletonDataTransfer* internalFindSkeletonJointByIndex(int jointIndex, SkeletonDataTransfer* skeletonDataTransfer);

public:
	// Retorna el joint correspondiente a partir del nombre del joint (y un joint root o padre)
	static SkeletonDataTransfer* findJointSkelletonUsingName(const std::string& inJointName, SkeletonDataTransfer* skeletonDataTransfer);

	// Devuelve el SkeletalDataTransfer correspondiente al joint index buscado
	static SkeletonDataTransfer* findSkeletonJointByIndex(int jointIndex, SkeletonDataTransfer* skeletonDataTransfer);
};