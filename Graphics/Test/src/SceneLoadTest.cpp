#include "SceneLoadTest.h"
#include "SceneLoader.h"

void SceneLoadTest::when(){
	this->scene = SceneLoader::loadFromXmlFile("assets/scene1.xml");
}

void SceneLoadTest::then(MockGraphicDevice* device){
	this->assertTrue("object position on scene", this->scene->getObjectManager()->getPositionByObjectId("child1") >= 0);
}