
varying highp vec2 textureCoords;

uniform mediump int pass;  //Indica que pasada realizar

uniform sampler2D textureToBlur;
uniform highp float positionIncrement[3];
uniform highp float weight[3];

uniform mediump vec2 imageSize;

mediump vec4 pass1(){
	mediump float dx = 1.0 / imageSize.x;
	mediump vec4 finalFrag = texture2D(textureToBlur, textureCoords) * weight[0];
		
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x + positionIncrement[1] * dx, textureCoords.y)) * weight[1];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x - positionIncrement[1] * dx, textureCoords.y)) * weight[1];
		
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x + positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x - positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	
	return finalFrag;	
}

mediump vec4 pass2(){
	mediump float dy = 1.0 / imageSize.y;
	mediump vec4 finalFrag = texture2D(textureToBlur, textureCoords) * weight[0];

	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[1] * dy)) * weight[1];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[1] * dy)) * weight[1];

	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[2] * dy)) * weight[2];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[2] * dy)) * weight[2];
	
	return finalFrag;		
}

void main()
{	if(pass == 1)
		gl_FragColor = pass1();
	else	
		gl_FragColor = pass2();
}
