#version 420

in vec3 VertexPosition; 

uniform mat4 PVMmatrix;

out vec4 positionToLight;

void main()
{	
	positionToLight = PVMmatrix * vec4(VertexPosition, 1.0f);
	
	gl_Position = positionToLight;
}
	