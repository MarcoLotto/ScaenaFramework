#version 300 es

in mediump vec3 normal;
in mediump vec2 TexCoord;

out vec3 NormalData;
out vec3 ColorData;
out vec3 MaterialData;

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