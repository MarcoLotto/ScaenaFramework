/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "ImportUtilities.h"
#include "FileProcessingException.h"

SkeletonDataTransfer* ImportUtilities::internalFindJointSkelletonUsingName(const std::string& inJointName, SkeletonDataTransfer* skeletonDataTransfer)
{
	// Busco en los hijos directos a ver si alguien matchea con el nombre	
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){	
		if ((*it)->jointName.compare(inJointName) == 0){
			return (*it);
		}
		// Busco en los hijos de este joint
		SkeletonDataTransfer* ret = internalFindJointSkelletonUsingName(inJointName, (*it));
		if(ret != NULL){
			return ret;
		}		
		++it;
	}	
	return NULL;
}

SkeletonDataTransfer* ImportUtilities::findJointSkelletonUsingName(const std::string& inJointName, SkeletonDataTransfer* skeletonDataTransfer)
{
	// Busco en los hijos directos a ver si alguien matchea con el nombre	
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){	
		if ((*it)->jointName.compare(inJointName) == 0){
			return (*it);
		}
		// Busco en los hijos de este joint
		SkeletonDataTransfer* ret = internalFindJointSkelletonUsingName(inJointName, (*it));
		if(ret != NULL){
			return ret;
		}		
		++it;
	}	
	throw new FileProcessingException("El FBX esta corrupto, no tiene sentido la relacion mesh joints/skelleton joints (buncando por nombre)");
}

SkeletonDataTransfer* ImportUtilities::internalFindSkeletonJointByIndex(int jointIndex, SkeletonDataTransfer* skeletonDataTransfer)
{
	// Busco en los hijos directos a ver si alguien matchea con el index	
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){	
		if ((*it)->jointIndex == jointIndex){
			return (*it);
		}
		// Busco en los hijos de este joint
		SkeletonDataTransfer* ret = internalFindSkeletonJointByIndex(jointIndex, (*it));
		if(ret != NULL){
			return ret;
		}		
		++it;
	}	
	return NULL;
}

SkeletonDataTransfer* ImportUtilities::findSkeletonJointByIndex(int jointIndex, SkeletonDataTransfer* skeletonDataTransfer)
{
	// Busco en los hijos directos a ver si alguien matchea con el index	
	list<SkeletonDataTransfer*>::iterator it = skeletonDataTransfer->getChilds()->begin();
	while(it != skeletonDataTransfer->getChilds()->end()){	
		if ((*it)->jointIndex == jointIndex){
			return (*it);
		}
		// Busco en los hijos de este joint
		SkeletonDataTransfer* ret = internalFindSkeletonJointByIndex(jointIndex, (*it));
		if(ret != NULL){
			return ret;
		}		
		++it;
	}	
	throw new FileProcessingException("El FBX esta corrupto, no tiene sentido la relacion mesh joints/skelleton joints (buscando por joint index)");
}