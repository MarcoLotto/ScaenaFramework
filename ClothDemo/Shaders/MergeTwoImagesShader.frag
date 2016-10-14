#version 420

in vec2 TexCoord;
out vec4 textureData;

// La informacion de las dos imagenes, y la transparecia de la segunda
uniform sampler2D image1;
uniform sampler2D image2;
uniform float transparency;
uniform int operation;

void main()
{	
	// Mergea dos imagenes, aplicando transperencia a la segunda imagen
	vec4 imageData1 = texture( image1, TexCoord );
	vec4 imageData2 = texture( image2, TexCoord );
	if(operation == 0){  // Promedio de pixels ponderado por transparencia
		textureData = mix(imageData1, imageData2, transparency);
	}
	else if(operation == 1){  // Multiplicacion de pixels
		textureData = imageData1 * imageData2;
	}
}