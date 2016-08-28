#version 300 es

precision mediump float;
precision mediump int;

in mediump vec3 normal;
in mediump vec2 TexCoord;

layout(location = 0) out vec3 NormalData;
layout(location = 1) out vec3 ColorData;
layout(location = 2) out vec3 MaterialData;

uniform sampler2D diffuseMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float Kb;
};
uniform Material mat;

void main()
{	
	NormalData = normal;
	ColorData = vec3(texture(diffuseMap, uvScale * (TexCoord + uvTranslation)));
	MaterialData.x = mat.Ka;
	MaterialData.y = mat.Kd;
	MaterialData.z = mat.Ks;
}