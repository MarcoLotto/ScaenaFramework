
attribute highp vec3 VertexPosition; 
attribute highp vec2 TextureCoords;

varying mediump vec2 textureCoords;

uniform mat4 PVMmatrix;

void main()
{	
	textureCoords = TextureCoords;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);	
}
	