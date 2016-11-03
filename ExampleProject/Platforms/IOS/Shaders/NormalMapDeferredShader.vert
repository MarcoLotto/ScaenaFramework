/*
#version 300 es

in highp vec3 VertexPosition; 
in highp vec3 VertexNormal; 
in highp vec2 VertexTexCoord; 
in highp vec3 VertexTangent;
in highp vec3 VertexBinormal;

out mediump vec3 normal;
out mediump vec2 TexCoord;
out mediump mat3 toObjectLocal;
out mediump mat3 normMatrix;

uniform mat4 PVMmatrix;
uniform mat3 normalMatrix;
 */

void main()
{
    /*
	normMatrix = normalMatrix;	
	normal = normalize(normalMatrix * VertexNormal);
	vec3 tangent = normalize(normalMatrix * VertexTangent);
	vec3 binormal = normalize(normalMatrix * VertexBinormal);
	toObjectLocal = mat3(tangent.x, binormal.x, normal.x,
						 tangent.y, binormal.y, normal.y,
						 tangent.z, binormal.z, normal.z ) ;	
	
	TexCoord = VertexTexCoord;

	gl_Position = PVMmatrix * vec4(VertexPosition, 1.0);
     */
}
	