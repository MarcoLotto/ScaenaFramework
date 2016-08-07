precision mediump float;
precision mediump int;

varying highp vec2 TexCoord;

uniform sampler2D image;
uniform highp int colorsCount;
uniform highp vec3 inputColors[10];
uniform highp vec3 outputColors[10];

void main()
{	
	highp vec4 imageDataRGBA = texture2D( image, TexCoord );
	highp vec3 imageDataRGB = vec3(imageDataRGBA);	

	// Si el pixel actual esta en la lista de input, lo cambio por el de la lista de output. Sino queda igual.
	for(int i=0; i < colorsCount; i++){
		gl_FragColor = vec4(float(imageDataRGB != inputColors[i]) * imageDataRGB + float(imageDataRGB == inputColors[i]) * outputColors[i], imageDataRGBA.w);
	}		
}