/**********************************
* SCAENA FRAMEWORK
* Author: Marco Andrés Lotto
* License: MIT - 2016
**********************************/

#pragma once
#include "glm/glm.hpp"
#include <string>
#include <vector>
using namespace std;

typedef glm::vec3 vec3;

class TeamBuilderDataTransfer{

public:
	string teamName;

	string shirtTemplate;
	string pantsTemplate;
	string socksTemplate;

	vector<vec3> shirtColors;
	vector<vec3> pantsColors;
	vector<vec3> socksColors;
};