#include "SceneLightsLoadTest.h"
#include "SceneLoader.h"

void SceneLightsLoadTest::when(){
	this->scene = SceneLoader::loadFromXmlFile("assets/scene1.xml");
}

void SceneLightsLoadTest::then(MockGraphicDevice* device){
	this->assertTrue("object light on scene", this->scene->getLightingManager()->getLights()->size() == 2);
}