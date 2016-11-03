/*
#version 300 es

in highp vec3 VertexPosition; 
in highp vec3 VertexNormal; 
in highp vec2 VertexTexCoord;
in highp vec4 VertexJointIndexes; 
in highp vec4 VertexJointWeights;  

out mediump vec3 normal;
out mediump vec2 TexCoord;

uniform mat4 PVMmatrix;
uniform mat3 normalMatrix;

uniform mat4 jointsTransformUniform[50];

vec4 getSkinnedVertexPosition(){
	// Calculo la nueva posicion como un promedio ponderado de aquellos joints que tienen peso sobre el vertice	 
	vec4 vertexOriginalPosition = vec4(VertexPosition, 1.0);
	vec4 p1 = jointsTransformUniform[int(VertexJointIndexes.x)] * vertexOriginalPosition;
	vec4 p2 = jointsTransformUniform[int(VertexJointIndexes.y)] * vertexOriginalPosition;
	vec4 p3 = jointsTransformUniform[int(VertexJointIndexes.z)] * vertexOriginalPosition;
	vec4 p4 = jointsTransformUniform[int(VertexJointIndexes.w)] * vertexOriginalPosition;	
	vec4 skinnedVertexPosition = p1 * VertexJointWeights.x + p2 * VertexJointWeights.y + p3 * VertexJointWeights.z + p4 * VertexJointWeights.w;	
	skinnedVertexPosition /= float(VertexJointWeights.x != 0.0) + float(VertexJointWeights.y != 0.0) + float(VertexJointWeights.z != 0.0) + float(VertexJointWeights.w != 0.0);
	return skinnedVertexPosition;
}
*/
void main()
{
    /*
	// Primero calculo la nueva posicion del vertice para coincidir con el skeleton actual
	vec4 skinnedVertexPosition = getSkinnedVertexPosition();
	
	normal = normalize(normalMatrix * VertexNormal); 
	TexCoord = VertexTexCoord;

	gl_Position = PVMmatrix * skinnedVertexPosition;
     */
}
	