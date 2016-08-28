#version 420

in vec3 VertexPosition; 
in vec2 TextureCoords;

out vec2 TexCoord;

uniform mat4 PVMmatrix;

void main()
{	
	TexCoord = TextureCoords;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);	
}