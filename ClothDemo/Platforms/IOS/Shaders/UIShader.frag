
varying mediump vec2 textureCoords;

uniform sampler2D textureData; 
uniform mediump vec2 partialSize;
uniform mediump vec2 positionIndex;
uniform mediump float transparency;

void main()
{	
	mediump vec2 finalCoord;
	finalCoord.x = textureCoords.x * partialSize.x + partialSize.x * positionIndex.x;  
	finalCoord.y = textureCoords.y * partialSize.y + partialSize.y * positionIndex.y;  
	gl_FragColor = texture2D(textureData, finalCoord);
	gl_FragColor.w *= transparency;
}