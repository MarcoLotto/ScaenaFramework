#version 300 es

precision mediump float;
precision mediump int;

in mediump vec3 normal;
in mediump vec2 TexCoord;
in mediump mat3 toObjectLocal;
in mediump mat3 normMatrix;

layout(location = 0) out vec3 NormalData;
layout(location = 1) out vec3 ColorData;
layout(location = 2) out vec3 MaterialData;

uniform sampler2D diffuseMap; 
uniform sampler2D normalMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

uniform float normalFactor;  //Factor de atenuacion del NormalMap
uniform float decayFactor;  //Factor de decaimiento

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float Kb;
};
uniform Material mat;

void main()
{	
	vec2 finalTextureCoord = uvScale * (TexCoord + uvTranslation);
	NormalData = vec3(texture( normalMap, finalTextureCoord )-vec4(0.5f, 0.5f, 0.5f, 0.0f));
	NormalData.x *= decayFactor;
	NormalData.y *= decayFactor;	
	NormalData = normalFactor * normalize(NormalData * toObjectLocal);
	ColorData = vec3(texture(diffuseMap, finalTextureCoord));
	MaterialData.x = mat.Ka;
	MaterialData.y = mat.Kd;
	MaterialData.z = mat.Ks;
}