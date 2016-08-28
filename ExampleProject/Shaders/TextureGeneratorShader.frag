#version 420

in vec2 TexCoord;
out vec4 textureData;

//Uniforms con los datos del vector a escribir en la textura y el tamaño de la textura en pixels (cuidado, harcodeado el tam max del vector)
uniform float valueVectorRed[100];
uniform float valueVectorGreen[100];
uniform float valueVectorBlue[100];
uniform vec2 size;

unsigned int getIndexByTextureCoord(){
	float xIndex = floor(TexCoord.x / (1.0f / size.x));
	float yIndex = floor(TexCoord.y / (1.0f / size.y));
	return unsigned int(size.x * yIndex + xIndex);
}

void main()
{	
	unsigned int i = getIndexByTextureCoord();
	textureData = vec4(valueVectorRed[i], valueVectorGreen[i], valueVectorBlue[i] , 1.0f );
}