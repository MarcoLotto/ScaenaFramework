#include "ExamplePipelineBuilder.h"
#include "RenderPipelineWithGeometryAndBackWrite.h"
#include "BloomStage.h"
#include "DeferredGeometryStage.h"
#include "PhongLightingStage.h"
#include "BlurStage.h"
#include "UserInterfaceStage.h"

RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration1(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Inicializamos una etapa de geometria forward
	ForwardGeometryStage* geometryStage = getGeometryForwardStage(uiController, scene, renderPipelineImp);
	
	// Asignamos los stage al pipeline
	renderPipelineImp->addGeometryStage(geometryStage);

	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

// El forward geometry stage se utiliza para renderizar geometria en una sola etapa (es decir la iluminacion se produce en el mismo shader de geometria)
ForwardGeometryStage* ExamplePipelineBuilder::getGeometryForwardStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline){
	vec2 size = uiController->getScreenSize();
	ForwardGeometryStage* geometryStage = new ForwardGeometryStage(size.x, size.y, false);
	geometryStage->setScene(scene);  // Le seteo la escena a renderizar al geometry stage
	scene->getLightingManager()->getShadowManager()->setRenderPipeline(pipeline);
	return geometryStage;
}

RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration2(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Inicializamos una etapa de geometria forward
	ForwardGeometryStage* geometryStage = getGeometryForwardStage(uiController, scene, renderPipelineImp);

	// Inicializamos una etapa de bloom (sobre-exposicion de luz)
	vec2 size = uiController->getScreenSize();
	BloomStage* bloomStage = new BloomStage(geometryStage, size.x, size.y, 8.0f);
	bloomStage->setLumThreshold(0.7f);

	// Asignamos los stage al pipeline
	renderPipelineImp->addGeometryStage(geometryStage);
	renderPipelineImp->addAfterGeometry(bloomStage);

	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration3(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Creamos una etapa de deferred geometry shading (la iluminación ocurre por separado de la geometria)
	DeferredGeometryStage* geometryStage = getGeometryDeferredStage(uiController, scene, renderPipelineImp);

	// Ahora creamos una etapa de iluminacion
	vec2 size = uiController->getScreenSize();
	PhongLightingStage* lightingStage = new PhongLightingStage(geometryStage, size.x, size.y);
	lightingStage->setScene(scene);

	// Cargamos las etapas al pipeline (geometria e iluminacion ocurren en diferentes etapas)
	renderPipelineImp->addGeometryStage(geometryStage);
	renderPipelineImp->addAfterGeometry(lightingStage);
	
	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

// El forward geometry stage se utiliza para renderizar geometria en una sola etapa (es decir la iluminacion se produce en el mismo shader de geometria)
DeferredGeometryStage* ExamplePipelineBuilder::getGeometryDeferredStage(UIController* uiController, Scene* scene, RenderPipelineWithGeometryAndBackWrite* pipeline){
	vec2 size = uiController->getScreenSize();
	DeferredGeometryStage* geometryStage = new DeferredGeometryStage(size.x, size.y);
	geometryStage->setScene(scene);  // Le seteo la escena a renderizar al geometry stage
	scene->getLightingManager()->getShadowManager()->setRenderPipeline(pipeline);
	return geometryStage;
}


RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration4(UIController* uiController, Scene* scene, bool nearDepthActive){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Creamos una etapa de deferred geometry shading (la iluminación ocurre por separado de la geometria)
	DeferredGeometryStage* geometryStage = getGeometryDeferredStage(uiController, scene, renderPipelineImp);

	// Ahora creamos una etapa de iluminacion
	vec2 size = uiController->getScreenSize();
	PhongLightingStage* lightingStage = new PhongLightingStage(geometryStage, size.x, size.y);
	lightingStage->setScene(scene);

	// Creamos un stage de depth of field
	DepthOfFieldStage* depthOfFieldStage = getDepthofFieldStage(geometryStage, lightingStage, size, nearDepthActive);
	depthOfFieldStage->setDepthAtBlurStart(7.0f);
	depthOfFieldStage->setBlurFalloff(1.0f);
	
	// Cargamos las etapas al pipeline (geometria e iluminacion ocurren en diferentes etapas)
	renderPipelineImp->addGeometryStage(geometryStage);
	renderPipelineImp->addAfterGeometry(lightingStage);
	renderPipelineImp->addAfterGeometry(depthOfFieldStage);
	
	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration5(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Creamos una etapa de deferred geometry shading (la iluminación ocurre por separado de la geometria)
	DeferredGeometryStage* geometryStage = getGeometryDeferredStage(uiController, scene, renderPipelineImp);

	// Agregamos una etapa de ambient occlussion (SSAO), y al resultado una de blur (A su vez esto sera utilizado por el stage de 
	// iluminacion para simular sombras en determinados lugares para iluminacion mas realista)
	vec2 size = uiController->getScreenSize();
	SSAORenderStage* ssaoStage = new SSAORenderStage(geometryStage, size.x, size.y);	
	BlurStage* ssaoBlurStage = new BlurStage(ssaoStage, size.x, size.y, 2.5f);

	// Ahora creamos una etapa de iluminacion y le damos la informacion del ssao procesado
	PhongLightingStage* lightingStage = new PhongLightingStage(geometryStage, size.x, size.y, ssaoBlurStage);
	lightingStage->setScene(scene);

	// Cargamos las etapas al pipeline (primero geometria, luego ssao, luego su blur y luego con todos los datos a iluminacion)
	renderPipelineImp->addGeometryStage(geometryStage);
	renderPipelineImp->addAfterGeometry(ssaoStage);
	renderPipelineImp->addAfterGeometry(ssaoBlurStage);
	renderPipelineImp->addAfterGeometry(lightingStage);
	
	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

DepthOfFieldStage* ExamplePipelineBuilder::getDepthofFieldStage(DeferredGeometryStage* geometryStage, BackBufferWriterStage* lastStage, vec2 size, bool nearDepthActive){
	DepthOfFieldStage* depthOfFieldStage = NULL;
	if(nearDepthActive)
		depthOfFieldStage = new DepthOfFieldStage(geometryStage, lastStage, size.x, size.y, 5.0f, 40.0f, false);
	else
		depthOfFieldStage = new DepthOfFieldStage(geometryStage, lastStage, size.x, size.y, 5.0f, 15.0f, true);
	depthOfFieldStage->setBlurFalloff(0.1f);
	return depthOfFieldStage;
}

RenderPipeline* ExamplePipelineBuilder::getRenderPipelineConfiguration6(UIController* uiController, Scene* scene){
	// Configuro el render pipeline
	RenderPipelineWithGeometryAndBackWrite* renderPipelineImp = new RenderPipelineWithGeometryAndBackWrite();

	// Inicializamos una etapa de geometria forward
	ForwardGeometryStage* geometryStage = getGeometryForwardStage(uiController, scene, renderPipelineImp);

	// Instaciamos una etapa de blur para aplicar sobre la imagen de geometria final
	vec2 size = uiController->getScreenSize();
	BlurStage* blurStage = new BlurStage(geometryStage, size.x, size.y, 8.0f);
	
	// Instanciamos una etapa de renderizado de UI para mostrar arriba de todo la interfaz de usuario
	UserInterfaceStage* userInterfaceStage = new UserInterfaceStage(blurStage, size.x, size.y);
	userInterfaceStage->setUiController(uiController);

	// Asignamos los stages al pipeline
	renderPipelineImp->addGeometryStage(geometryStage);
	renderPipelineImp->addAfterGeometry(blurStage);
	renderPipelineImp->addAfterGeometry(userInterfaceStage);

	// Terminado el pipeline, lo mando a cargar
	renderPipelineImp->updatePipelineScheme();  
	return renderPipelineImp;
}

