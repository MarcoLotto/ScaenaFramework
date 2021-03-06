
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
	
	posicion = vec3(VMmatrix * skinnedVertexPosition);	
	vec3 normal = normalize(normalMatrix * VertexNormal);
	vec3 tangent = normalize(normalMatrix * VertexTangent);
	vec3 binormal = normalize(normalMatrix * VertexBinormal);
	toObjectLocal = mat3(tangent.x, binormal.x, normal.x,
						 tangent.y, binormal.y, normal.y,
						 tangent.z, binormal.z, normal.z ) ;	
	
	TexCoord = VertexTexCoord;	

	gl_Position = PVMmatrix * skinnedVertexPosition;
}