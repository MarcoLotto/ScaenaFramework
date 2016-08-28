
attribute highp vec3 VertexPosition; 
attribute highp vec3 VertexNormal; 
attribute highp vec2 VertexTexCoord; 
attribute highp vec3 VertexTangent;
attribute highp vec3 VertexBinormal;

varying mediump vec3 position;
varying mediump vec2 TexCoord;
varying mediump mat3 toObjectLocal;

uniform mat4 PVMmatrix;
uniform mat4 VMmatrix;
uniform mat3 normalMatrix;

void main()
{
	vec3 normal = normalize(normalMatrix * VertexNormal);
	vec3 tangent = normalize(normalMatrix * VertexTangent);
	vec3 binormal = normalize(normalMatrix * VertexBinormal);
	toObjectLocal = mat3(tangent.x, binormal.x, normal.x,
						 tangent.y, binormal.y, normal.y,
						 tangent.z, binormal.z, normal.z ) ;	
	
	TexCoord = VertexTexCoord;

	position = vec3(VMmatrix * vec4(VertexPosition, 1.0));
	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);
}
	