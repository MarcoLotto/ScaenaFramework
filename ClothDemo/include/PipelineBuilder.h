/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "RenderPipeline.h"
#include "UIController.h"
#include "ForwardGeometryStage.h"

class PipelineBuilder {

private:
	static ForwardGeometryStage* getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline);
	
public:
	static RenderPipeline* getRenderPipelineConfigForward(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineForUIOnly(UIController* uiController);

};