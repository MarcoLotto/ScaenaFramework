#version 300 es

in vec3 VertexPosition; 
in vec2 VertexDefaultTexCoord; 

out vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 proyMatrix;

void main()
{
	TexCoord = VertexDefaultTexCoord;
	gl_Position = proyMatrix * modelMatrix * vec4(VertexPosition, 1.0f);
}
	