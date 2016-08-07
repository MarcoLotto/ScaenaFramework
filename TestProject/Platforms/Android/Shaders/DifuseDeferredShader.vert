#version 300 es

in highp vec3 VertexPosition; 
in highp vec3 VertexNormal; 
in highp vec2 VertexTexCoord; 

out mediump vec3 normal;
out mediump vec2 TexCoord;

uniform mat4 PVMmatrix;
uniform mat3 normalMatrix;

void main()
{
	normal = normalize(normalMatrix * VertexNormal); 
	TexCoord = VertexTexCoord;

	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);
}
	