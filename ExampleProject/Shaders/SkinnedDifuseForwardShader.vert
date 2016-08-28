#version 420

in vec3 VertexPosition; 
in vec3 VertexNormal; 
in vec2 VertexTexCoord; 
in vec4 VertexJointIndexes; 
in vec4 VertexJointWeights; 

out vec3 position;
out vec3 normal;
out vec2 TexCoord;

uniform mat4 PVMmatrix;
uniform mat4 VMmatrix;
uniform mat3 normalMatrix;

uniform mat4 jointsTransformUniform[50];

vec4 getSkinnedVertexPosition(){
	// Calculo la nueva posicion como un promedio ponderado de aquellos joints que tienen peso sobre el vertice	 
	vec4 vertexOriginalPosition = vec4(VertexPosition, 1.0f);	
	vec4 p1 = jointsTransformUniform[int(VertexJointIndexes.x)] * vertexOriginalPosition;
	vec4 p2 = jointsTransformUniform[int(VertexJointIndexes.y)] * vertexOriginalPosition;
	vec4 p3 = jointsTransformUniform[int(VertexJointIndexes.z)] * vertexOriginalPosition;
	vec4 p4 = jointsTransformUniform[int(VertexJointIndexes.w)] * vertexOriginalPosition;	
	vec4 skinnedVertexPosition = p1 * VertexJointWeights.x + p2 * VertexJointWeights.y + p3 * VertexJointWeights.z + p4 * VertexJointWeights.w;	
	skinnedVertexPosition /= float(VertexJointWeights.x != 0.0f) + float(VertexJointWeights.y != 0.0f) + float(VertexJointWeights.z != 0.0f) + float(VertexJointWeights.w != 0.0f);
	return skinnedVertexPosition;
}

void main()
{	
	// Primero calculo la nueva posicion del vertice para coincidir con el skeleton actual
	vec4 skinnedVertexPosition = getSkinnedVertexPosition();
	
	// Ahora realizo el renderizado comun
	normal = normalize(normalMatrix * VertexNormal); 
	TexCoord = VertexTexCoord;
	position = vec3(VMmatrix * skinnedVertexPosition);
	gl_Position = PVMmatrix * skinnedVertexPosition;
}
	