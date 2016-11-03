#version 420

in vec3 VertexPosition; 
in vec3 VertexNormal; 
in vec2 VertexTexCoord; 

out vec3 position;
out vec3 normal;
out vec2 TexCoord;

uniform mat4 PVMmatrix;
uniform mat4 VMmatrix;
uniform mat3 normalMatrix;

void main()
{
	normal = normalize(normalMatrix * VertexNormal); 
	TexCoord = VertexTexCoord;
	position = vec3(VMmatrix * vec4(VertexPosition, 1.0f));
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);
}
	