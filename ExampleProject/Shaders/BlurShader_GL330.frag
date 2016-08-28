#version 330

in vec2 textureCoords;

out vec4 BlurredData;

uniform int pass;  //Indica que pasada realizar

uniform sampler2D textureToBlur;
uniform float positionIncrement[3];
uniform float weight[3];
//uniform float positionIncrement[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
//uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

uniform vec2 imageSize;

vec4 pass1(){
	float dx = 1.0f / imageSize.x;
	vec4 finalFrag = texture(textureToBlur, textureCoords) * weight[0];
		
	finalFrag += texture(textureToBlur, vec2(textureCoords.x + positionIncrement[1] * dx, textureCoords.y)) * weight[1];
	finalFrag += texture(textureToBlur, vec2(textureCoords.x - positionIncrement[1] * dx, textureCoords.y)) * weight[1];
		
	finalFrag += texture(textureToBlur, vec2(textureCoords.x + positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	finalFrag += texture(textureToBlur, vec2(textureCoords.x - positionIncrement[2] * dx, textureCoords.y)) * weight[2];
	
	return finalFrag;	
}

vec4 pass2(){
	float dy = 1.0f / imageSize.y;
	vec4 finalFrag = texture(textureToBlur, textureCoords) * weight[0];

	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[1] * dy)) * weight[1];
	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[1] * dy)) * weight[1];

	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[2] * dy)) * weight[2];
	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[2] * dy)) * weight[2];
	
	return finalFrag;		
}

void main()
{	if(pass == 1)
		BlurredData = pass1();
	else	
		BlurredData = pass2();
}