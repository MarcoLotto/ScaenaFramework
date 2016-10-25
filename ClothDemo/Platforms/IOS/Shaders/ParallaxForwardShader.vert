
attribute highp vec3 VertexPosition; 
attribute highp vec3 VertexNormal; 
attribute highp vec2 VertexTexCoord; 
attribute highp vec3 VertexTangent;
attribute highp vec3 VertexBinormal;

varying mediump vec3 posicion;
varying mediump vec2 TexCoord;
varying mediump mat3 toObjectLocal;

uniform mat4 VMmatrix;
uniform mat4 PVMmatrix;
uniform mat3 normalMatrix;

void main()
{
	posicion = vec3(VMmatrix * vec4(VertexPosition, 1.0));
	vec3 normal = normalize(normalMatrix * VertexNormal);
	vec3 tangent = normalize(normalMatrix * VertexTangent);
	vec3 binormal = normalize(normalMatrix * VertexBinormal);
	toObjectLocal = mat3(tangent.x, binormal.x, normal.x,
						 tangent.y, binormal.y, normal.y,
						 tangent.z, binormal.z, normal.z ) ;	
	
	TexCoord = VertexTexCoord;	

	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);
}