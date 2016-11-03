varying highp vec2 TexCoord;

uniform sampler2D image;
uniform highp int colorsCount;
uniform highp float colorTolerance;
uniform highp vec3 inputColors[10];
uniform highp vec3 outputColors[10];

void main()
{	
	highp vec4 imageDataRGBA = texture2D( image, TexCoord );
	highp vec3 imageDataRGB = vec3(imageDataRGBA);

	// Si el pixel actual esta en la lista de input, lo cambio por el de la lista de output. Sino queda igual.
	gl_FragColor = imageDataRGBA;
	for(int i=0; i < colorsCount; i++){
		highp vec3 minColor = imageDataRGB - vec3(colorTolerance);
		highp vec3 maxColor = imageDataRGB + vec3(colorTolerance);
		if( (inputColors[i].x > minColor.x && inputColors[i].x < maxColor.x) && 
			(inputColors[i].y > minColor.y && inputColors[i].y < maxColor.y) && 
			(inputColors[i].z > minColor.z && inputColors[i].z < maxColor.z)){
			gl_FragColor = vec4(outputColors[i], imageDataRGBA.w);
		}		
	}
}