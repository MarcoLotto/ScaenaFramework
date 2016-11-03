#version 420

in vec2 TexCoord;
out vec4 OcclusionData;

//Uniforms con los datos de la stage de geometria
uniform sampler2D DepthTex;
uniform sampler2D NormalTex;

// La textura con la información random para rotar el kernel
uniform sampler2D randomTexture;
uniform vec2 randomTextureSize;

// El tamaño de las texturas de posicion y normal
uniform vec2 imageSize;

// EL kernel con las posiciones a probar occlusion
uniform float sampleKernel[384];
// La cantidad de samples a probar (+1 por cada triada de cordenada)
uniform unsigned int samplesCount;

//Tengo que contar con la matriz de proyeccion de la escena para pasar el depth con perspectiva al depth lineal
uniform mat4 sceneProj;


// Dada una coordenada de textura de un punto, devuelve su posicion respecto a la camara
vec3 getPositionFromPoint(vec2 coordToFetch){
    float depth = texture( DepthTex, coordToFetch ).x * 2.0f - 1.0f;
    float viewDepth = -sceneProj[3][2] / (depth + sceneProj[2][2]);
	vec2 nDc = (2.0f * coordToFetch - vec2(1.0f)) * viewDepth;
    vec3 viewPosition = vec3(nDc.x / -sceneProj[0][0], nDc.y / -sceneProj[1][1], viewDepth);
	return -viewPosition;
}

// Dada una coordenada de textura de un punto, devuelve la matriz de transformacion orientada a la normal del punto
mat3 getToObjectLocalMatrixFromPoint(vec2 coordToFetch, vec3 randomVector){
	randomVector = normalize(randomVector);
	vec3 normal = -normalize(vec3(texture( NormalTex, coordToFetch)) * 2.0f - 1.0f);
	vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
	vec3 bitangent = cross(normal, tangent);
	return mat3(tangent, bitangent, normal);
}

// Lee la textura de valores random y consigue el valor random del fragmento
vec3 getRandomVector(){
	vec2 tamPixel = vec2(1.0f / imageSize.x, 1.0f / imageSize.y);
	vec2 finalTexCoord = vec2(TexCoord.x / (randomTextureSize.x * tamPixel.x), TexCoord.y / (randomTextureSize.y * tamPixel.y));
	return 2.0f*texture( randomTexture, finalTexCoord ).xyz - 1.0f;
}

// Aplica las tranformaciones de la normal al kernel, para que este se oriente respecto a la misma
vec3 transformSampleKernel(vec3 evaluatedPointPosition, vec3 kernel, mat3 toObjectLocalMatrix){
	return toObjectLocalMatrix * kernel + evaluatedPointPosition;	
}

// Proyecta las muestras (en view space) en la pantalla (screen space), para asi conseguir las coordenas de textura en donde hacer fetch
vec2 getProjectedSampleCoords(vec3 orientedKernel){
	vec4 offset = vec4(orientedKernel, 1.0);
    offset = sceneProj * offset;
    offset.xy /= offset.w;
    return offset.xy * 0.5 + 0.5;	
}

void main()
{	
	// Consigo los valores random para la rotacion del kernel
	vec3 randomVector = getRandomVector();	
	
	// Armo la matriz de pasaje a espacio local del fragmento (para orientar hacia la normal)
	mat3 toObjectLocalMatrix = getToObjectLocalMatrixFromPoint(TexCoord, randomVector);
	
	vec3 evaluatedPointPosition = getPositionFromPoint(TexCoord);
	float occlusionFactor = 1.0f;
	float occlusionDecrement = (1.0f / (1.0f*samplesCount)); 
	for(int i = 0; i < 3*samplesCount; i += 3){
		// Roto el kernel segun la normal y el vector de random, y lo proyecto en la pantalla para conseguir las coordenadas en donde hacer fetch
		vec3 orientedKernel = transformSampleKernel(evaluatedPointPosition, vec3(sampleKernel[i], sampleKernel[i+1], sampleKernel[i+2]), toObjectLocalMatrix);
		vec2 sampleCoords = getProjectedSampleCoords(orientedKernel);
		
		// Consigo el depth de la muestra
		float sampleDepth = getPositionFromPoint(sampleCoords).z;
			
		// Si la profundidad es mayor a la del sample, entonces el punto esta occluded (salvo que sea mucha la diferencia)	
		float hasAO = (sampleDepth < orientedKernel.z ? 1.0f : 0.0f);
		float aoRadiusAffects = (abs(orientedKernel.z - sampleDepth) < 20.0f ? 1.0 : 0.0);		
		occlusionFactor -= occlusionDecrement * (hasAO * aoRadiusAffects + (1.0f-aoRadiusAffects) * hasAO * 0.5f);			
	}
	float finalSaturate = 2.0f*occlusionFactor + 2.0f*occlusionFactor*occlusionFactor - 2.0f*occlusionFactor*occlusionFactor*occlusionFactor;
	OcclusionData = vec4(0.0f, clamp(finalSaturate, 0.0, 1.0), 0.0f , 1.0f );
}