#version 420

in vec2 TexCoord;
out vec4 textureData;

uniform sampler2D image;
uniform int colorsCount;
uniform float colorTolerance;
uniform vec3 inputColors[10];
uniform vec3 outputColors[10];

void main()
{	
	vec4 imageDataRGBA = texture( image, TexCoord );
	vec3 imageDataRGB = vec3(imageDataRGBA);	

	// Si el pixel actual esta en la lista de input, lo cambio por el de la lista de output. Sino queda igual.
	textureData = imageDataRGBA;
	for(int i=0; i < colorsCount; i++){
		vec3 minColor = imageDataRGB - vec3(colorTolerance);
		vec3 maxColor = imageDataRGB + vec3(colorTolerance);
		if( (inputColors[i].x > minColor.x && inputColors[i].x < maxColor.x) && 
			(inputColors[i].y > minColor.y && inputColors[i].y < maxColor.y) && 
			(inputColors[i].z > minColor.z && inputColors[i].z < maxColor.z)){
			textureData = vec4(outputColors[i], imageDataRGBA.w);
			return;
		}		
	}			
}