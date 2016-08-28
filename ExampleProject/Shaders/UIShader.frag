#version 420

in vec2 textureCoords;

uniform sampler2D textureData; 
uniform vec2 partialSize;
uniform vec2 positionIndex;
uniform float transparency;

out vec4 FragColor;

void main()
{	
	vec2 finalCoord;
	finalCoord.x = textureCoords.x * partialSize.x + partialSize.x * positionIndex.x;  
	finalCoord.y = textureCoords.y * partialSize.y + partialSize.y * positionIndex.y;  
	FragColor = texture(textureData, finalCoord);
	FragColor.w *= transparency;
}