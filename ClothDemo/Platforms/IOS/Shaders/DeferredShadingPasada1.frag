#version 420

in vec3 posicion;
in vec3 normal;
in vec2 TexCoord;

uniform sampler2D texturaDataDifusa; 

out vec4 FragColor;
out vec3 PositionData;
out vec3 NormalData;
out vec3 ColorData;
out vec3 MaterialData;

 struct Material{
	float Ka;
	float Kd;
	float Ks;
	float brillo;
};
uniform Material mat1;

void main()
{	
	PositionData = posicion;
	NormalData = normal;
	ColorData = vec3(texture(texturaDataDifusa, TexCoord));
	MaterialData.x = mat1.Ka;
	MaterialData.y = mat1.Kd;
	MaterialData.z = mat1.Ks;	
}