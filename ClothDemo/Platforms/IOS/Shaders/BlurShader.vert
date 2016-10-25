#version 300 es

in vec3 VertexPosition; 
in vec2 TextureCoords;

out vec2 textureCoords;

uniform mat4 PVMmatrix;

void main()
{	
	textureCoords = TextureCoords;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);	
}
	
