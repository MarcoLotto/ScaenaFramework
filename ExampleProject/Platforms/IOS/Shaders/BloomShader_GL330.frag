
varying highp vec2 textureCoords;

uniform mediump float lumThreshold; // Luminance threshold

uniform mediump int pass;  //Indica que pasada realizar

uniform sampler2D textureToBlur;
uniform sampler2D originalTexture;

uniform highp float positionIncrement[3];
uniform highp float weight[3];
uniform mediump vec2 imageSize;

//La funcion luma, procesa un valor respecto del color del fragmento
float luma( vec3 color ) {
	return 0.2126 * color.r + 0.7152 * color.g +
	0.0722 * color.b;
}

//En la primera pasada selecciono aquellos pixels en un rango de color intenso
vec4 pass1(){
	vec4 finalFrag = texture2D(textureToBlur, textureCoords);	
	return finalFrag * clamp(luma(vec3(finalFrag)) - lumThreshold, 0.0f, 1.0f) * (1.0f / (1.0f - lumThreshold));	
}

vec4 pass2(){
	float dx = 1.0f / imageSize.x;
	vec4 finalFrag = texture2D(textureToBlur, textureCoords) * weight[0];
		
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x + positionIncrement[1] * dx, textureCoords.y)) * weight[1];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x - positionIncrement[1] * dx, textureCoords.y)) * weight[1];
		
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x + positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x - positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	
	return finalFrag;	
}

vec4 pass3(){
	//Primero leo el valor de la textura original
	vec4 originalData = texture2D(originalTexture, textureCoords);

	float dy = 1.0f / imageSize.y;
	vec4 finalFrag = texture2D(textureToBlur, textureCoords) * weight[0];

	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[1] * dy)) * weight[1];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[1] * dy)) * weight[1];

	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[2] * dy)) * weight[2];
	finalFrag += texture2D(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[2] * dy)) * weight[2];

	return originalData + finalFrag;		
}

void main()
{	
	if(pass == 1)
		gl_FragColor = pass1();
	else if(pass == 2)
		gl_FragColor = pass2();
	else	
		gl_FragColor = pass3();	
}