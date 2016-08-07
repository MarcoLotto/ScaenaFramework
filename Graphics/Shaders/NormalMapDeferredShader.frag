#version 420

in vec3 normal;
in vec2 TexCoord;
in mat3 toObjectLocal;
in mat3 normMatrix;

uniform sampler2D diffuseMap; 
uniform sampler2D normalMap; 

uniform vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform vec2 uvScale;

uniform float normalFactor;  //Factor de atenuacion del NormalMap
uniform float decayFactor;  //Factor de decaimiento

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