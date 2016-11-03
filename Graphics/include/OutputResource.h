/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "FrameBuffer.h"
#include "MemoryTexture.h"
class RenderStage;

class OutputResource{

private:
	MemoryTexture* outputTexture;
	RenderStage* freeWriteLockStage;
	int indexCache;
	bool unlockAllowed;

public:
	OutputResource(MemoryTexture* outputTexture){ 
		this->outputTexture = outputTexture;
		this->freeWriteLockStage = NULL;
		this->indexCache = 0;
		this->unlockAllowed = true;
	}
	void setOutputTexture(MemoryTexture* texture){ this->outputTexture = texture; }
	MemoryTexture* getOutputTexture(){ return this->outputTexture; }
	
	void setFreeWriteLockStage(RenderStage* stage){ this->freeWriteLockStage = stage; }
	RenderStage* getFreeWriteLockStage(){ return this->freeWriteLockStage; }
	
	void setIndexCache(int index){ this->indexCache = index; }
	int getIndexChache(){ return this->indexCache; }

	void setUnlockAllowed(bool allowsUnlock){ this->unlockAllowed = allowsUnlock; }
	bool isUnlockAllowed(){ return this->unlockAllowed; }
};
