/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "SkinnedAnimation.h"
#include <map>
using namespace std;

class SkinnedAnimationRepository{

private:
	static SkinnedAnimationRepository* instance;
	SkinnedAnimationRepository(){};

	// Mapea un digest de un skeleton con las posibles animaciones que se le pueden aplicar.
	map<unsigned int, map<string, SkinnedAnimation*>*> animationMethods;  
	
public:
	static SkinnedAnimationRepository* getInstance();

	// Agrega una animación al repositorio
	void addAnimation(SkinnedAnimation* animation);

	// Dado un esqueleto y un nombre de animación, busca esa animación entre las aplicables para ese esqueleto y devuelve un clon de la misma.
	// Si no existe ninguna animación con dicho nombre aplicable para el esqueleto, devuelve NULL.
	SkinnedAnimation* getAnimation(MeshSkeleton* skeleton, string animationName);

	// Borra (limpia fisicamente) todo el contenido del repositorio
	void cleanRepository();
};