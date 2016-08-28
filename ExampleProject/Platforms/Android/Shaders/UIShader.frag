precision mediump float;
precision mediump int;

varying mediump vec2 textureCoords;

uniform sampler2D textureData; 
uniform vec2 partialSize;
uniform vec2 positionIndex;
uniform float transparency;

void main()
{	
	vec2 finalCoord;
	finalCoord.x = textureCoords.x * partialSize.x + partialSize.x * positionIndex.x;  
	finalCoord.y = textureCoords.y * partialSize.y + partialSize.y * positionIndex.y;  
	gl_FragColor = texture2D(textureData, finalCoord);
	gl_FragColor.w *= transparency;
}