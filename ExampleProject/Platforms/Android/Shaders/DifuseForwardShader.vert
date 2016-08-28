
attribute highp vec3 VertexPosition; 
attribute highp vec3 VertexNormal; 
attribute highp vec2 VertexTexCoord; 

varying mediump vec3 position;
varying mediump vec3 normal;
varying mediump vec2 TexCoord;

uniform highp mat4 PVMmatrix;
uniform highp mat4 VMmatrix;
uniform highp mat3 normalMatrix;

void main()
{
	normal = normalize(normalMatrix * VertexNormal); 
	TexCoord = VertexTexCoord;
	position = vec3(VMmatrix * vec4(VertexPosition, 1.0));
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);
}

