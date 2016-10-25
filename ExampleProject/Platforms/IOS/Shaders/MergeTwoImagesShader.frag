varying highp vec2 TexCoord;

// La informacion de las dos imagenes, la transparecia de la segunda y la operacion a realizar
uniform sampler2D image1;
uniform sampler2D image2;
uniform highp float transparency;
uniform highp int operation;

void main()
{	
	// Mergea dos imagenes, aplicando transperencia a la segunda imagen
	highp vec4 imageData1 = texture2D( image1, TexCoord );
	highp vec4 imageData2 = texture2D( image2, TexCoord );
	if(operation == 0){  // Promedio de pixels ponderado por transparencia
		gl_FragColor = mix(imageData1, imageData2, transparency);
	}
	else if(operation == 1){  // Multiplicacion de pixels
		gl_FragColor = imageData1 * imageData2;
	}
}