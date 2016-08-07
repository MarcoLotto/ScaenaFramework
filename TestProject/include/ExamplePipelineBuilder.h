#pragma once
#include "RenderPipeline.h"
#include "UIController.h"

#include "ForwardGeometryStage.h"
#include "DeferredGeometryStage.h"
#include "DepthOfFieldStage.h"

static class ExamplePipelineBuilder {

private:
	static ForwardGeometryStage* getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline);
	static DeferredGeometryStage* getGeometryDeferredStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline);
	static DepthOfFieldStage* getDepthofFieldStage(DeferredGeometryStage* geometryStage, BackBufferWriterStage* lastStage, vec2 size, bool nearDepthActive);

public:
	static RenderPipeline* getRenderPipelineConfiguration1(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfiguration2(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfiguration3(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfiguration4(UIController* uiController, Scene* scene, bool nearDepthActive);
	static RenderPipeline* getRenderPipelineConfiguration5(UIController* uiController, Scene* scene);
	static RenderPipeline* getRenderPipelineConfiguration6(UIController* uiController, Scene* scene);

};