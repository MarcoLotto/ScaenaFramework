#version 420

in vec3 VertexPosition; 
in vec3 VertexNormal; 
in vec2 VertexTexCoord; 
in vec3 VertexTangent;
in vec3 VertexBinormal;

out vec3 posicion;
out vec3 normal;
out vec2 TexCoord;
out mat3 toObjectLocal;
out mat3 normMatrix;

uniform mat4 VMmatrix;
uniform mat4 PVMmatrix;
uniform mat3 normalMatrix;

void main()
{
	normMatrix = normalMatrix;
	posicion = vec3(VMmatrix * vec4(VertexPosition, 1.0f));	
	normal = normalize(normalMatrix * VertexNormal);
	vec3 tangent = normalize(normalMatrix * VertexTangent);
	vec3 binormal = normalize(normalMatrix * VertexBinormal);
	toObjectLocal = mat3(tangent.x, binormal.x, normal.x,
						 tangent.y, binormal.y, normal.y,
						 tangent.z, binormal.z, normal.z ) ;	
	
	TexCoord = VertexTexCoord;	

	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0f);
}