#pragma once
#include "RenderPipeline.h"
#include "UIController.h"

#include "ForwardGeometryStage.h"
#include "DeferredGeometryStage.h"
#include "DepthOfFieldStage.h"

class RenderPipelineBuilder {

private:
	static ForwardGeometryStage* getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline);
	static DeferredGeometryStage* getGeometryDeferredStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline);
	static DepthOfFieldStage* getDepthofFieldStage(DeferredGeometryStage* geometryStage, BackBufferWriterStage* lastStage, vec2 size, bool nearDepthActive);

public:
	static RenderPipeline* getRenderPipelineConfigForward(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfigForwardAndBloom(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfigDeferred(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfigDeferredAndDepthOfField(UIController* uiController, Scene* scene, bool nearDepthActive);
	static RenderPipeline* getRenderPipelineConfigDeferredAndSSAO(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfigForwardAndBlur(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfigForwardAndUI(UIController* uiController, Scene* scene);

};