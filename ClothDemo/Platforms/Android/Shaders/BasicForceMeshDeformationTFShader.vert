#version 300 es

in highp vec3 VertexPosition; 

out mediump vec3 VertexPositionOutput;

uniform vec3 forceDirection;
uniform float forceIntensity;
uniform vec3 staticPoint;
uniform float staticStrength;

// Shader de transformation feedback para dada una fuerza y un punto de apoyo, deformar un mesh (o cualquier buffer)
void main()
{	
	VertexPositionOutput = VertexPosition;	
	VertexPositionOutput += vec3(forceDirection*forceIntensity * (distance(VertexPosition, staticPoint) / staticStrength));	
}
	