#version 420

in vec3 normal;
in vec2 TexCoord;

uniform sampler2D diffuseMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

out vec3 NormalData;
out vec3 ColorData;
out vec3 MaterialData;

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