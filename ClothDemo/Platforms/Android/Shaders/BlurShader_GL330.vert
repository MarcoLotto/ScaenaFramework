
attribute highp vec3 VertexPosition; 
attribute highp vec2 TextureCoords;

varying highp vec2 textureCoords;

uniform highp mat4 PVMmatrix;

void main()
{	
	textureCoords = TextureCoords;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);	
}
	
