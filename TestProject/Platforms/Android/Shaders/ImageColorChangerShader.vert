
attribute highp vec3 VertexPosition; 
attribute highp vec2 TextureCoords;

varying highp vec2 TexCoord;

uniform highp mat4 PVMmatrix;

void main()
{	
	TexCoord = TextureCoords;
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);	
}