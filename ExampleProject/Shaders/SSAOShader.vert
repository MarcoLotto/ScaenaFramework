#version 420

in vec3 VertexPosition; 
in vec2 TextureCoords;

out vec2 TexCoord;

uniform mat4 PVMmatrix;

void main()
{	
	// Invierto la coordenada x de las coordenadas de textura para que coincida la textura de AO en la etapa de iluminación (Sino se espeja)
	TexCoord.x = TextureCoords.x;
	TexCoord.y = TextureCoords.y;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);	
}