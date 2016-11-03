/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SkinnedAnimationKeyFrame.h"
#include "MeshSkeletonInterpolator.h"
#include <vector>
using namespace std;

class SkinnedAnimation{

private:
	string name;
	vector<SkinnedAnimationKeyFrame*> keyFrames;
	MeshSkeletonInterpolator keyFramesInterpolator;
	SkinnedAnimationKeyFrame* smoothKeyFrame;
	bool smoothKeyFrameExecuted;
	int actualKeyIndex;
	float actualKeyFrameTime;
	bool hasLoop;
	bool reverseAnimationActive;  
	bool inverseAnimationLoopEnabled;
	void nextKeyFrame();

public:
	SkinnedAnimation(string name);
	virtual ~SkinnedAnimation();

	void setName(string name){ this->name =name; }
	string getName(){ return this->name; }


	void addKeyFrame(SkinnedAnimationKeyFrame* keyFrame){ this->keyFrames.push_back(keyFrame); }
	void setSmoothKeyFrame(SkinnedAnimationKeyFrame* keyFrame){ this->smoothKeyFrame = keyFrame; }
	vector<SkinnedAnimationKeyFrame*>* getKeyFrames(){ return &this->keyFrames; }

	// Llamar en cada frame para actualizar el estado de la animación
	void update();

	// Consigue la posicion actual del esqueleto en la animación o el keyframe (interpolado) actual
	MeshSkeleton* getActualPose();
	SkinnedAnimationKeyFrame* getActualKeyFrame();

	// Vuelve a cero la animación
	void reset();

	// Indica si la animacion vuelve a empezar una vez finalizada
	void setLoopAnimation(bool hasLoop){  this->hasLoop = hasLoop; }

	// Indica si la animacion vuelve a empezar pero en sentido inverso una vez finalizada
	void setInverseLoopAnimation(bool hasInverseLoop){  this->hasLoop = hasInverseLoop; this->inverseAnimationLoopEnabled = hasInverseLoop; }

	// Setea el tiempo total que durara la animacion, ponderando respectivamente la duracion de cada keyframe
	void setTotalAnimationTime(float timeInSeconds);

	SkinnedAnimation* clone();
};