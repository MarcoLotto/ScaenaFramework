#version 300 es

precision mediump float;
precision mediump int;

in vec2 textureCoords;

out vec4 BlurredData;

subroutine vec4 blurPassType();
subroutine uniform blurPassType blurPass;

// La atextura a la que aplicar blur y la textura con la informacion de depth
uniform sampler2D textureToBlur;
uniform sampler2D depthTexture;

uniform float positionIncrement[3];
uniform float weight[3];

//Desde que depth empieza a aplicarse blur y si el blur se aplica hacia atras o hacia adelante de la pantalla
uniform float depthAtBlurStart;
uniform int blurToFront;

// El factor de decaimiento del blur (es decir que tan rapido llegara el blur a su maximo una vez alcanzado depthAtBlurStart) 
uniform float blurFalloff;

//Tengo que contar con la matriz de proyeccion de la escena para pasar de la textura de depth a valores lineales
uniform mat4 sceneProj;

// El tamaño de la imagen a aplicar blur
uniform vec2 imageSize;

// A partir de la textura de depth se recontruye la posicion del fragmento en el subespacio de view-model
float getFragmentLinearDepth(){ 
    float depth = texture( depthTexture, textureCoords ).x * 2.0 - 1.0;
    return sceneProj[3][2] / (depth + sceneProj[2][2]);
}

// Calculo que atenuacion de blur se debe tener para este valor de depth
float getDepthAttenuation(){
	float linearDepth = getFragmentLinearDepth();
	float distanceBetweenDepths = abs(linearDepth - depthAtBlurStart) * blurFalloff;
	float frontDepthAttenuation = float(blurToFront) * (distanceBetweenDepths * float(linearDepth > depthAtBlurStart));
	float backDepthAttenuation = float(1.0 - blurToFront) * (distanceBetweenDepths * float(linearDepth < depthAtBlurStart));
	return min(frontDepthAttenuation + backDepthAttenuation, 1.0);
}	

subroutine(blurPassType)
vec4 pass1(){	
	// La atenuacion en el blur para este depth
	float depthAttenuation = getDepthAttenuation();
		
	float dx = 1.0 / imageSize.x;
	vec4 finalFrag = texture(textureToBlur, textureCoords) * (weight[0] * depthAttenuation + (1.0 - depthAttenuation));
		
	finalFrag += texture(textureToBlur, vec2(textureCoords.x + positionIncrement[1] * dx, textureCoords.y)) * weight[1] * depthAttenuation;
	finalFrag += texture(textureToBlur, vec2(textureCoords.x - positionIncrement[1] * dx, textureCoords.y)) * weight[1] * depthAttenuation;
		
	finalFrag += texture(textureToBlur, vec2(textureCoords.x + positionIncrement[2] * dx, textureCoords.y)) * weight[2] * depthAttenuation;
	finalFrag += texture(textureToBlur, vec2(textureCoords.x - positionIncrement[2] * dx, textureCoords.y)) * weight[2] * depthAttenuation;
	
	return finalFrag;	
}

subroutine(blurPassType)
vec4 pass2(){
	// La atenuacion en el blur para este depth
	float depthAttenuation = getDepthAttenuation();
		
	float dy = 1.0 / imageSize.y;
	vec4 finalFrag = texture(textureToBlur, textureCoords) * (weight[0] * depthAttenuation + (1.0 - depthAttenuation));

	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[1] * dy)) * weight[1] * depthAttenuation;
	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[1] * dy)) * weight[1] * depthAttenuation;

	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y + positionIncrement[2] * dy)) * weight[2] * depthAttenuation;
	finalFrag += texture(textureToBlur, vec2(textureCoords.x, textureCoords.y - positionIncrement[2] * dy)) * weight[2] * depthAttenuation;
	
	return finalFrag;		
}

// Shader de Depth of Blur
void main()
{	
	BlurredData = blurPass();	
}