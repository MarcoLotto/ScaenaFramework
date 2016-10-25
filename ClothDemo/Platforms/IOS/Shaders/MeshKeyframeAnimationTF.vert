#version 420

in vec3 EndVertexPosition; 
in vec3 EndNormalPosition;
in vec3 EndTangentPosition;
in vec3 EndBinormalPosition;

in vec3 StartVertexPosition; 
in vec3 StartNormalPosition; 
in vec3 StartTangentPosition; 
in vec3 StartBinormalPosition; 

out vec3 VertexPositionOutput;
out vec3 NormalPositionOutput;
out vec3 TangentPositionOutput;
out vec3 BinormalPositionOutput;

uniform float animationPoint;

// Shader de transformation feedback para dados dos mesh, interpolarlos a travez del tiempo
void main()
{	
	VertexPositionOutput = (1.0f - animationPoint) * StartVertexPosition + animationPoint * EndVertexPosition;
	NormalPositionOutput = (1.0f - animationPoint) * StartNormalPosition + animationPoint * EndNormalPosition;
	TangentPositionOutput = (1.0f - animationPoint) * StartTangentPosition + animationPoint * EndTangentPosition;
	BinormalPositionOutput = (1.0f - animationPoint) * StartBinormalPosition + animationPoint * EndBinormalPosition;	
}
	