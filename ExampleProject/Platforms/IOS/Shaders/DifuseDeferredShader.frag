/*
#version 300 es

in mediump vec3 normal;
in mediump vec2 TexCoord;

layout(location = 0) out mediump vec3 NormalData;
layout(location = 1) out mediump vec3 ColorData;
layout(location = 2) out mediump vec3 MaterialData;

uniform sampler2D diffuseMap; 

uniform mediump vec2 uvTranslation;  // Transformaciones de la coordenadas UV
uniform mediump vec2 uvScale;

 struct Material{
	mediump float Ka;
	mediump float Kd;
	mediump float Ks;
	mediump float Kb;
};
uniform Material mat;
 */

void main()
{
    /*
	NormalData = normal;
	ColorData = vec3(texture(diffuseMap, uvScale * (TexCoord + uvTranslation)));
	MaterialData.x = mat.Ka;
	MaterialData.y = mat.Kd;
	MaterialData.z = mat.Ks;
     */
}