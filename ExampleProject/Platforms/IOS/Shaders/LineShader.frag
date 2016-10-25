#version 420

in vec3 position;

out vec3 NormalData;
out vec3 ColorData;
out vec3 MaterialData;

uniform vec3 colorDeLinea;

void main(){
	NormalData = vec3(0.0f);
	ColorData = vec3(colorDeLinea);
	MaterialData.x = 1.0f;
	MaterialData.y = 0.0f;
	MaterialData.z = 0.0f;	
}