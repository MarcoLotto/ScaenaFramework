/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#include "SceneCache.h"
#include "SceneLoader.h"

SceneCache* SceneCache::instance = NULL;

SceneCache* SceneCache::getInstance(){
	if(instance == NULL){
		instance = new SceneCache();
	}
	return instance;
}


Scene* SceneCache::loadFromXmlFile(string filename){
	if(this->cache.find(filename) == this->cache.end()){
		// La escena no está en cache, la cargo y la guardo en cache
		Scene* scene = SceneLoader::loadFromXmlFile(filename.c_str());
		this->cache[filename] = scene;
	}
	return this->cache[filename];
}